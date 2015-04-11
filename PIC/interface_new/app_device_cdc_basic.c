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

#include <stdint.h>
#include <string.h>
#include <stddef.h>

#include <usb/usb.h>

#include <app_device_cdc_basic.h>
#include <usb_config.h>

/** VARIABLES ******************************************************/

//static bool buttonPressed;
//static char buttonMessage[] = "Button pressed.\r\n";
static uint8_t Out_Buffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t In_Buffer[CDC_DATA_IN_EP_SIZE];

/*unsigned char  NextUSBOut;
unsigned char    NextUSBOut;
unsigned char    LastRS232Out;  // Number of characters in the buffer
unsigned char    RS232cp;       // current position within the buffer
unsigned char RS232_Out_Data_Rdy = 0;
USB_HANDLE  lastTransmission;*/


/*********************************************************************
* Function: void APP_DeviceCDCEmulatorInitialize(void);
*
* Overview: Initializes the demo code
*
* PreCondition: None
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceCDCEmulatorInitialize()
{
    CDCInitEP();
    
    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 19200;

    unsigned char i;

// 	 Initialize the arrays
	for (i=0; i<sizeof(Out_Buffer); i++)
    {
		Out_Buffer[i] = 0;
    }
}

/*********************************************************************
* Function: void APP_DeviceCDCEmulatorTasks(void);
*
* Overview: Keeps the demo running.
*
* PreCondition: The demo should have been initialized and started via
*   the APP_DeviceCDCEmulatorInitialize() and APP_DeviceCDCEmulatorStart() demos
*   respectively.
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceCDCEmulatorTasks()
{
	
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1))
		return;
	

	unsigned char length = getsUSBUSART(In_Buffer,1);
	if(length > 0)
	{
		Out_Buffer[0] = 5;
		Out_Buffer[1] = 'C';
		Out_Buffer[2] = 'U';
		Out_Buffer[3] = 'B';
		Out_Buffer[4] = 'O';
		putUSBUSART(&Out_Buffer[0], 5);

		PORTC = 0b0100;
	}

    CDCTxService();
}