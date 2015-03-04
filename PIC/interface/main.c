#include "i2c_connector.h"
#include "HardwareProfile.h"
#include "usb_connector.h"
//#include <p18F14K50.h>
#include <delays.h> 
#include <string.h>
#include <eep.h>

#include <CommonConstants.h>
#include <common/common.h>
//14K50
#pragma config CPUDIV = NOCLKDIV
#pragma config USBDIV = OFF
#pragma config FOSC   = HS
#pragma config PLLEN  = ON
#pragma config FCMEN  = OFF
#pragma config IESO   = OFF
#pragma config PWRTEN = OFF
#pragma config BOREN  = OFF
#pragma config BORV   = 30
#pragma config WDTEN  = OFF
#pragma config WDTPS  = 32768
#pragma config MCLRE  = OFF
#pragma config HFOFST = OFF
#pragma config STVREN = ON
#pragma config LVP    = OFF
#pragma config XINST  = OFF
#pragma config BBSIZ  = OFF
#pragma config CP0    = OFF
#pragma config CP1    = OFF
#pragma config CPB    = OFF
#pragma config WRT0   = OFF
#pragma config WRT1   = OFF
#pragma config WRTB   = OFF
#pragma config WRTC   = OFF
#pragma config EBTR0  = OFF
#pragma config EBTR1  = OFF
#pragma config EBTRB  = OFF 

#pragma udata
char out_buffer[8];
const char protocolId[] = PROTOCOL_ID;
#pragma code

unsigned char g_state = 255;

void Response(char const * answer, unsigned answerLength)
{
	out_buffer[0] = 1 + answerLength;
	memcpy(out_buffer+1, answer, answerLength);
	PutUsbData(out_buffer, out_buffer[0]);	
}

void ResponseChar(unsigned char answer)
{
	out_buffer[0] = 2;
	out_buffer[1] = answer;
	PutUsbData(out_buffer, 2);
}

void SetState(char const *data)
{
	g_state = data[3];
	Write_b_eep(0, g_state);
	Busy_eep ();
	
	PutStateI2C(g_state);
	out_buffer[0] = 1;
	PutUsbData(out_buffer, 1);
	PORTC = 0b0101;
}

void SetDescendentMode(char const *data)
{
	PutCommandI2C(COMMAND_CHANGE_MODE, data +3, 1);
	out_buffer[0] = 1;
	PutUsbData(out_buffer, 1);
}

void GetState(char const *data)
{
	out_buffer[0] = g_state;
	PutUsbData(out_buffer, 1);
}

void usbDataReaded(char const *data, int size)
{
	FunctionId functionId;
	
	if (0 == size)
		return;

	functionId = data[2];
	switch (functionId)
	{
		case FID_GET_PROTOCOL_ID:
			Response(protocolId, sizeof(protocolId)-1);
			break;

		//case FID_GET_PARAMETERS:
			//GetParameters(data);
			//break;
		
		//case FID_GET_NEIGHBOR_ADDRESS:
			//GetNeighborAddress(data);
			//break;
		case FID_SET_STATE:
			SetState(data);
			break;
		case FID_GET_STATE:
			GetState(data);
			break;
		case FID_GET_MODE:
			ResponseChar(GetCommandI2C(COMMAND_GET_CURRENT_MODE));
			break;
		case FID_SET_MODE:
			SetDescendentMode(data);
			break;
	}
}

void main(void)
{  
	LATC=0;
	TRISC = 0;

	I2CInit();
	InitializeUSB();

	LATB=0;
	LATBbits.LATB7= 0;
	TRISBbits.TRISB7 = 0; 
	PORTBbits.RB7 = 1; //is connected bite

	g_state = Read_b_eep(0);
	PutI2C(I2C_MESSAGE_TYPE_DATA, 0, &g_state, 1);
	while(1)
    {
		ProcessUSB(usbDataReaded);
    }
}//end main
