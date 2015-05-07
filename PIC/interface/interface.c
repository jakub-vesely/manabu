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

void UsbDataRead()
{
	unsigned char retVal = 0;
	if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1))
		return;

	unsigned char length = getsUSBUSART(buffer,CDC_DATA_IN_EP_SIZE);
	if (0 != length)
	{
		switch (buffer[2])
		{
		case FID_GET_PROTOCOL_ID:
			Response((unsigned char *)protocolId, sizeof(protocolId)-1);
			break;
		case FID_SET_STATE:
			//SetState(data[3]);
			ResponseChar(0);
			break;
		case FID_GET_STATE:
			ResponseChar(g_state);
			break;
		case FID_GET_MODE:
			if (!GetCommandI2C(COMMAND_GET_CURRENT_MODE, &retVal))
				retVal = 0xff;
			ResponseChar(retVal);
			break;
		case FID_SET_MODE:
			//SetDescendentMode(data[3]);
			ResponseChar(0);
			break;

		case FID_COMMAND_FLASH_GET_VERSION:
			if (!GetCommandI2C(COMMAND_FLASH_GET_VERSION, &retVal))
				retVal = 0xff;
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
			if (!GetCommandI2C(COMMAND_FLASH_CHECKSUM, &retVal))
				retVal = 0xff;
			ResponseChar(retVal);
			break;
		case FID_COMMAND_FLASH_SET_BOOT_FLAG:
			PutCommandI2C(COMMAND_FLASH_SET_BOOT_FLAG, buffer+3, 1);
			ResponseChar(0);
			break;
		}
	}

	CDCTxService();
}

MAIN_RETURN main(void)
{
	OSCCONbits.IRCF = 0b1111; //16MHz
	while (!OSCSTATbits.HFIOFS);
	
	#if defined(USE_INTERNAL_OSC)
		OSCCON = 0xFC;  //HFINTOSC @ 16MHz, 3X PLL, PLL enabled
		ACTCON = 0x90;  //Active clock tuning enabled for USB
    #endif
		ANSELC = 0;
		TRISC = 0;

	I2cMasterInit();
	
    USBDeviceInit();
    USBDeviceAttach();
    
    while(1)
    {
		SYSTEM_Tasks();
		if( USBGetDeviceState() < CONFIGURED_STATE || USBIsDeviceSuspended() == true )
			continue;

		UsbDataRead();

    }//end while
}//end main

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

/*******************************************************************************
 End of File
*/
