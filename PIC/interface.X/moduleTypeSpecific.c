#include "moduleTypeSpecific.h"
#include <system_common.h>
#include <system.h>
#include <usb_config.h>

#include <usb/usb.h>
#include <usb/usb_device.h>
#include <usb/usb_device_cdc.h>

#include <CommonConstants.h>

#include <common/common.h>
#include <common/i2c.h>

#define CHARGING_LED_OUTPUT LATC2
#define CHARGING_N_STATE_INPUT RC5

static uint8_t s_buffer[CDC_DATA_OUT_EP_SIZE];
static const char s_protocolId[] = PROTOCOL_ID;

void Response(unsigned char const * answer, unsigned answerLength)
{
	int i = 0;
	s_buffer[0] = (1 + answerLength);
	for (; i < answerLength; i++)
		s_buffer[i+1] = answer[i];
	putUSBUSART(&s_buffer[0], s_buffer[0]);
}

void ResponsePrepared(unsigned answerLength)
{
	s_buffer[0] = (1 + answerLength);
	putUSBUSART(&s_buffer[0], s_buffer[0]);
}

void ResponseChar(unsigned char answer)
{
	s_buffer[0] = 2;
	s_buffer[1] = answer;
	putUSBUSART(s_buffer, 2);
}
unsigned char GetFromI2C(MessageId command, unsigned char* data, unsigned char count)
{
	unsigned tryCounter = TO_OUTPUT_MAX_TRAY;
	while (--tryCounter != 0) //I will repeat it couple of times because output module could be connected to an output and bss b sending a message
	{
		if (GetCommandFromI2C(command, data, count))
			return count;

		InvertOutput(); //may be output module is inverted
	}

	return 0;
}
void UsbDataRead()
{
	if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1))
		return;

	unsigned char length = getsUSBUSART(s_buffer,CDC_DATA_IN_EP_SIZE);
	if (0 != length)
	{
		unsigned char deviceId = s_buffer[1];
		switch (s_buffer[2])
		{
		case MID_GET_PROTOCOL_ID:
			Response((unsigned char *)s_protocolId, sizeof(s_protocolId)-1);
			break;
		case MID_SET_STATE:
			g_inState = *(unsigned *)(s_buffer+3);
			g_stateChanged = true;
			ResponseChar(0);
			break;
		case MID_GET_STATE:
			if (0 == deviceId)
				Response((unsigned char const*)&g_inState, 2);
			else
				ResponsePrepared(GetFromI2C(MID_GET_STATE, s_buffer + 1, 2));
			break;
		case MID_COMMAND_FLASH_GET_VERSION:
			ResponsePrepared(GetFromI2C(MID_COMMAND_FLASH_GET_VERSION, s_buffer+ 1, 1));
			break;
		case MID_COMMAND_FLASH_END:
			SendCommand(MID_COMMAND_FLASH_END, NULL, 0);
			ResponseChar(0);
			break;
		case MID_COMMAND_FLASH_ADDRESS:
			//in the case there is not a program it is the first programming command there is a bootloader so no status message will be accepted
			g_stateMessageEnabled = false;
			SendCommand(MID_COMMAND_FLASH_ADDRESS, s_buffer+3, 2);
			ResponseChar(0);
			break;
		case MID_COMMAND_FLASH_LATCH_WORD:
			SendCommand(MID_COMMAND_FLASH_LATCH_WORD, s_buffer+3, 2);
			ResponseChar(0);
			break;
		case MID_COMMAND_FLASH_WRITE_WORD:
			SendCommand(MID_COMMAND_FLASH_WRITE_WORD, s_buffer+3, 2);
			ResponseChar(0);
			break;
		case MID_COMMAND_FLASH_CHECKSUM:
			ResponsePrepared(GetFromI2C(MID_COMMAND_FLASH_CHECKSUM, s_buffer + 1, 1));
			break;
		case MID_COMMAND_FLASH_SET_BOOT_FLAG:
			//in the case there is a program it is the first programming command which cause a reset of module, no more state message will be accepted
			//it is also the last proggramming command. the main programm is alread running ane status messages will be accepted again
			g_stateMessageEnabled = (g_stateMessageEnabled ? false : true);

			SendCommand(MID_COMMAND_FLASH_SET_BOOT_FLAG, s_buffer+3, 1);
			ResponseChar(0);
			break;
		case MID_GET_MODULE_TYPE:
			if (0 == deviceId)
				ResponseChar(GetModuleType());
			else
				ResponsePrepared(GetFromI2C(MID_GET_MODULE_TYPE, s_buffer + 1, 1));
			break;
		case MID_GET_MODE:
			if (0 == deviceId)
				ResponseChar(g_persistant.mode);
			else
				ResponsePrepared(GetFromI2C(MID_GET_MODE, s_buffer + 1, 1));
			break;
		case MID_SET_MODE:
			if (0 == deviceId)
				SetMode(s_buffer[3]);
			else
				SendCommand(MID_SET_MODE, s_buffer+3, 1);
			ResponseChar(0);
			break;
		default:
			SendCommand(s_buffer[2], s_buffer+3, 1);
			break;
		}
	}

	CDCTxService();
}

void APP_DeviceCDCEmulatorInitialize()
{
    CDCInitEP();

    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 19200;
}

bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch( (int) event )
    {
        case EVENT_TRANSFER:
            break;

        case EVENT_SOF:
            break;

        case EVENT_SUSPEND:
            break;

        case EVENT_RESUME:
            break;

        case EVENT_CONFIGURED:
            /* When the device is configured, we can (re)initialize the
             * demo code. */
            APP_DeviceCDCEmulatorInitialize();
            break;

        case EVENT_SET_DESCRIPTOR:
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The HID driver
             * needs to check to see if the request was for it. */
            USBCheckCDCRequest();
            break;

        case EVENT_BUS_ERROR:
            break;

        case EVENT_TRANSFER_TERMINATED:
            break;

        default:
            break;
    }
    return true;
}

void ProcessCommandModuleTypeSpecific()
{
}
void ProcessStateChangedModuleTypeSpecific()
{
	g_outState = g_inState;
}

void ModuleTypeSpecificInit()
{
	SWDTEN = false; //watchdog is disabled
	WDTCONbits.WDTPS = 0b0111;

	OSCCON = 0xFC;  //HFINTOSC @ 16MHz, 3X PLL, PLL enabled
	ACTCON = 0x90;  //Active clock tuning enabled for USB
	ANSELC = 0;

	LATC = 0;
	TRISC = 0;

	I2cMasterInit();

    USBDeviceInit();
    USBDeviceAttach();
	g_inState = STATE_MAX;

	TRISC5 = true; //charging state
	TRISC2 = false; //LED
	g_inState = 50;
}


void ProcessModuleFunctionality()
{
	//CHARGING_LED_OUTPUT = !CHARGING_N_STATE_INPUT;
	
	SYSTEM_Tasks();

	if ((USBGetDeviceState() < DEFAULT_STATE || USBIsDeviceSuspended()))
	{
		SWDTEN = true; //watchdog is enabled
#asm
		SLEEP;
#endasm
		SWDTEN = false; //watchdog is disabled
		return;
	}


	if( USBGetDeviceState() < CONFIGURED_STATE || USBIsDeviceSuspended() == true )
		return;

	//RC2 = false;
	UsbDataRead();
	
}

unsigned char GetModuleType()
{
	return TYPE_USB_INTERFACE;
}
