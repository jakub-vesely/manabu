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

static uint8_t buffer[CDC_DATA_OUT_EP_SIZE];
static const char protocolId[] = PROTOCOL_ID;

void Response(unsigned char * answer, unsigned answerLength)
{
	int i = 0;
	buffer[0] = (1 + answerLength);
	for (; i < answerLength; i++)
		buffer[i+1] = answer[i];
	putUSBUSART(&buffer[0], buffer[0]);
}

void ResponseChar(unsigned char answer)
{
	buffer[0] = 2;
	buffer[1] = answer;
	putUSBUSART(buffer, 2);
}
unsigned char GetFromI2C(MessageId command)
{
	unsigned  counter = 1000; //there should probably be less
	unsigned char retVal = 0;
	while (0 != counter--)
	{
		if (GetCommandI2C(command, &retVal))
			return retVal;
	}
	return 0x0;
}
void UsbDataRead()
{
	if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1))
		return;

	unsigned char length = getsUSBUSART(buffer,CDC_DATA_IN_EP_SIZE);
	if (0 != length)
	{
		unsigned char deviceId = buffer[1];
		switch (buffer[2])
		{
		case MID_GET_PROTOCOL_ID:
			Response((unsigned char *)protocolId, sizeof(protocolId)-1);
			break;
		case MID_SET_STATE:
			g_state = buffer[3];
			g_stateChanged = true;
			ResponseChar(0);
			break;
		case MID_GET_STATE:
			if (0 == deviceId)
				ResponseChar(g_state);
			else
				ResponseChar(GetFromI2C(MID_GET_STATE));
			break;
		case MID_COMMAND_FLASH_GET_VERSION:
			ResponseChar(GetFromI2C(MID_COMMAND_FLASH_GET_VERSION));
			break;
		case MID_COMMAND_FLASH_END:
			PutCommandI2C(MID_COMMAND_FLASH_END, NULL, 0);
			ResponseChar(0);
			break;
		case MID_COMMAND_FLASH_ADDRESS:
			PutCommandI2C(MID_COMMAND_FLASH_ADDRESS, buffer+3, 2);
			ResponseChar(0);
			break;
		case MID_COMMAND_FLASH_LATCH_WORD:
			PutCommandI2C(MID_COMMAND_FLASH_LATCH_WORD, buffer+3, 2);
			ResponseChar(0);
			break;
		case MID_COMMAND_FLASH_WRITE_WORD:
			PutCommandI2C(MID_COMMAND_FLASH_WRITE_WORD, buffer+3, 2);
			ResponseChar(0);
			break;
		case MID_COMMAND_FLASH_CHECKSUM:
			ResponseChar(GetFromI2C(MID_COMMAND_FLASH_CHECKSUM));
			break;
		case MID_COMMAND_FLASH_SET_BOOT_FLAG:
			PutCommandI2C(MID_COMMAND_FLASH_SET_BOOT_FLAG, buffer+3, 1);
			ResponseChar(0);
			break;
		case MID_GET_MODULE_TYPE:
			if (0 == deviceId)
				ResponseChar(GetModuleType());
			else
				ResponseChar(GetFromI2C(MID_GET_MODULE_TYPE));
			break;
		default:
			PutCommandI2C(buffer[2], buffer+3, 1);
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

void ProcessStateChangedModuleTypeSpecific()
{
}

void ModuleTypeSpecificInit()
{
	#if defined (_PIC18F14K50_H_)
	ANSEL = 0;
	ANSELH = 0;
#else
	OSCCON = 0xFC;  //HFINTOSC @ 16MHz, 3X PLL, PLL enabled
	ACTCON = 0x90;  //Active clock tuning enabled for USB
	ANSELC = 0;
#endif
	LATC = 0;
	TRISC = 0;

	I2cMasterInit();

    USBDeviceInit();
    USBDeviceAttach();
	g_state = 0;
}


void ProcessModuleFunctionality()
{
	SYSTEM_Tasks();
	if( USBGetDeviceState() < CONFIGURED_STATE || USBIsDeviceSuspended() == true )
		return;

	UsbDataRead();
}

unsigned char GetModuleType()
{
	return TYPE_USB_INTERFACE;
}
