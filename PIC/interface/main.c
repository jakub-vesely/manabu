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
unsigned const char protocolId[] = PROTOCOL_ID;
#pragma code

unsigned char g_state = 255;

void Response(unsigned char const * answer, unsigned answerLength)
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

void SetState(unsigned char state)
{
	g_state = state;
	Write_b_eep(0, g_state);
	Busy_eep ();
	
	PutStateI2C(g_state);
	out_buffer[0] = 1;
	PutUsbData(out_buffer, 1);
	PORTC = 0b0101;
}

void SetDescendentMode(unsigned char data)
{
	PutCommandI2C(COMMAND_CHANGE_MODE, &data, 1);
	out_buffer[0] = 1;
	PutUsbData(out_buffer, 1);
}

void GetState(unsigned char const *data)
{
	out_buffer[0] = g_state;
	PutUsbData(out_buffer, 1);
}

void usbDataReaded(unsigned char const *data, int size)
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
			SetState(data[3]);
			break;
		case FID_GET_STATE:
			GetState(data);
			break;
		case FID_GET_MODE:
			ResponseChar(GetCommandI2C(COMMAND_GET_CURRENT_MODE));
			break;
		case FID_SET_MODE:
			SetDescendentMode(data[3]);
			break;
	}
}

unsigned int ADC_Read10bit(void)
{
    unsigned int result;

	ADCON0bits.CHS = 9;
    ADCON0bits.GO = 1;              // Start AD conversion
    while(ADCON0bits.NOT_DONE);     // Wait for conversion

    result = ADRESH;
    result <<=8;
    result |= ADRESL;

    return result;
}


void main(void)
{  
	unsigned char potValue = 0;
	unsigned char pressed = 0;
	unsigned char mode = 0;
	LATA = 0;
	LATC = 0;
	TRISA = 0;
	TRISC = 0;
	ANSELH = 0x0;
	ANSEL = 0x0;

	I2CInit();
	InitializeUSB();

	TRISCbits.TRISC7 = 1;
	ANSELHbits.ANS9 = 1;
	
	ADCON0bits.CHS = 9;
	ADCON0bits.ADON = 1;
	ADCON1=0;
	ADCON2=0x3E;
	ADCON2bits.ADFM = 1;

	LATB=0;
	LATBbits.LATB7= 0;
	TRISBbits.TRISB7 = 0; 
	PORTBbits.RB7 = 1; //is connected bite

	g_state = Read_b_eep(0);
	PutI2C(I2C_MESSAGE_TYPE_DATA, 0, &g_state, 1);
	potValue = g_state;
	while(1)
    {
		unsigned int potValue10 = ADC_Read10bit();

		if (PORTAbits.RA3 == 0)
		{
			if (!pressed)
			{
				mode++;
				if (mode > 3)
					mode = 1;
				SetDescendentMode(mode);

				pressed = 1;
			}
		}
		else if (pressed)
			pressed = 0;

		
		if (potValue10 / 4 != potValue)
		{
			potValue = potValue10 / 4;
			SetState(potValue);
		}
		ProcessUSB(usbDataReaded);
    }
}//end main
