/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

/** INCLUDES *******************************************************/
#include <system.h>
#include <system_config.h>

#include <app_device_cdc_basic.h>
#include <i2c_connector.h>
#include <usb/usb.h>
#include <usb/usb_device.h>
#include <usb/usb_device_cdc.h>

#include <CommonConstants.h>
static uint8_t buffer[CDC_DATA_OUT_EP_SIZE];
static const char protocolId[] = PROTOCOL_ID;
/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/
bool Continue()
{
#if defined(USB_POLLING)
		// Interrupt or polling method.  If using polling, must call
		// this function periodically.  This function will take care
		// of processing and responding to SETUP transactions
		// (such as during the enumeration process when you first
		// plug in).  USB hosts require that USB devices should accept
		// and process SETUP packets in a timely fashion.  Therefore,
		// when using polling, this function should be called
		// regularly (such as once every 1.8ms or faster** [see
		// inline code comments in usb_device.c for explanation when
		// "or faster" applies])  In most cases, the USBDeviceTasks()
		// function does not take very long to execute (ex: <100
		// instruction cycles) before it returns.
		USBDeviceTasks();
#endif


	/* If the USB device isn't configured yet, we can't really do anything
	 * else since we don't have a host to talk to.  So jump back to the
	 * top of the while loop. */
	if( USBGetDeviceState() < CONFIGURED_STATE )
	{
		/* Jump back to the top of the while loop. */
		return true;
	}

	/* If we are currently suspended, then we need to see if we need to
	 * issue a remote wakeup.  In either case, we shouldn't process any
	 * keyboard commands since we aren't currently communicating to the host
	 * thus just continue back to the start of the while loop. */
	if( USBIsDeviceSuspended()== true )
	{
		/* Jump back to the top of the while loop. */
		return true;
	}
		
	return false;
}

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

void UsbDataRead()
{
	unsigned char retVal;
	if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1))
		return;

	unsigned char length = getsUSBUSART(buffer,CDC_DATA_IN_EP_SIZE);
	if (0 != length)
	{
		switch (buffer[2])
		{
		case FID_GET_PROTOCOL_ID:
			Response(protocolId, sizeof(protocolId)-1);
			PORTC = 0b0011;
			break;
		case FID_SET_STATE:
			//SetState(data[3]);
			ResponseChar(0);
			break;
		case FID_GET_STATE:
			//GetState(data);
			ResponseChar(0); //FIXME
			break;
		case FID_GET_MODE:
			//if (!GetCommandI2C(COMMAND_GET_CURRENT_MODE, &retVal))
			//	retVal = 0xff;
			//ResponseChar(retVal);
			break;
		case FID_SET_MODE:
			//SetDescendentMode(data[3]);
			ResponseChar(0);
			break;

		case FID_COMMAND_FLASH_GET_VERSION:
			GetCommandI2C(COMMAND_FLASH_GET_VERSION, &retVal);
			ResponseChar(retVal);
			break;
		case FID_COMMAND_FLASH_END:
			PutCommandI2C(COMMAND_FLASH_END, NULL, 0);
			ResponseChar(0);
			break;
		case FID_COMMAND_FLASH_ADDRESS:
			PutCommandI2C(COMMAND_FLASH_ADDRESS, buffer+3, 2);
			ResponseChar(0);
			break;
		case FID_COMMAND_FLASH_LATCH_WORD:
			PutCommandI2C(COMMAND_FLASH_LATCH_WORD, buffer+3, 2);
			ResponseChar(0);
			break;
		case FID_COMMAND_FLASH_WRITE_WORD:
			PutCommandI2C(COMMAND_FLASH_WRITE_WORD, buffer+3, 2);
			ResponseChar(0);
			break;
		case FID_COMMAND_FLASH_CHECKSUM:
			GetCommandI2C(COMMAND_FLASH_CHECKSUM, &retVal);
			ResponseChar(retVal);
			break;
		case FID_COMMAND_FLASH_LOAD_CHECK:
			PutCommandI2C(COMMAND_FLASH_LOAD_CHECK, buffer+3, 1);
			ResponseChar(0);
			break;
		}
	}

	CDCTxService();
}

MAIN_RETURN main(void)
{
	TRISC = 0;
	LATC = 0;

	I2CInit();
	USBDeviceInit();
	USBDeviceAttach();

	while(1)
	{
		
		SYSTEM_Tasks();
		if (Continue())
			continue;

		UsbDataRead();

    }//end while
}//end main




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

/*******************************************************************************
 End of File
*/
