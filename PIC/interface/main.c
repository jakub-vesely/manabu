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


/*void main(void)
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
*/

/*unsigned const g_data1[8] = {0x107e, 0x0020, 0x2a03, 0x0021, 0x128e, 0x0020, 0x168e , 0x2a07};
unsigned const g_data2[30] =
{
	0x107e, 0x0020, 0x2a03, 0x0021, 0x128e, 0x0020, 0x168e, 0x30d0,
	0x00f0, 0x3007, 0x00f1, 0x30ff, 0x07f0, 0x30ff, 0x3df1, 0x1ff1,
	0x2a0b, 0x0020, 0x128e, 0x30d0, 0x00f0, 0x3007, 0x00f1, 0x30ff,
	0x07f0, 0x30ff, 0x3df1, 0x1bf1, 0x2a05, 0x2a17
};*/

 unsigned const g_data1[8] = {0x107e, 0x0020, 0x2903, 0x0021, 0x128e, 0x0020, 0x168e , 0x2907};
unsigned const g_data2[30] =
{
	0x107e, 0x0020, 0x2903, 0x0021, 0x128e, 0x0020, 0x168e, 0x30d0,
	0x00f0, 0x3007, 0x00f1, 0x30ff, 0x07f0, 0x30ff, 0x3df1, 0x1ff1,
	0x290b, 0x0020, 0x128e, 0x30d0, 0x00f0, 0x3007, 0x00f1, 0x30ff,
	0x07f0, 0x30ff, 0x3df1, 0x1bf1, 0x2905, 0x2917
};
 
void main()
{
	unsigned address = 0x100;
	unsigned i = 20000;
	unsigned pos = 0;
	unsigned count = (PORTAbits.RA3 == 0) ? sizeof(g_data1)/sizeof(unsigned) : sizeof(g_data2)/sizeof(unsigned);
	unsigned char checkSum = 0;
	unsigned version;
	I2CInit();

	PORTC = 0b0001;
	version = GetCommandI2C(COMMAND_FLASH_START);

	for (;pos < count; pos++)
	{
		if (!(pos % 16))
		{
			PutCommandI2C(COMMAND_FLASH_ADDRESS, &address, 2);
			checkSum += address & 0xff;
			checkSum += address >> 8;
			address += 16;
		}
		if (pos != count-1 && ((pos+ 1) % 16))
		{
			if (PORTAbits.RA3 == 0)
			{
				PutCommandI2C(COMMAND_FLASH_LATCH_WORD, &(g_data1[pos]), 2);
				checkSum += g_data1[pos] & 0xff;
				checkSum += g_data1[pos] >> 8;
			}
			else
			{
				PutCommandI2C(COMMAND_FLASH_LATCH_WORD, &(g_data2[pos]), 2);
				checkSum += g_data2[pos] & 0xff;
				checkSum += g_data2[pos] >> 8;
			}
		}
		else
		{
			if (PORTAbits.RA3 == 0)
			{
				PutCommandI2C(COMMAND_FLASH_WRITE_WORD, &(g_data1[pos]), 2);
				checkSum += g_data1[pos] & 0xff;
				checkSum += g_data1[pos] >> 8;
			}
			else
			{
				PutCommandI2C(COMMAND_FLASH_WRITE_WORD, &(g_data2[pos]), 2);
				checkSum += g_data2[pos] & 0xff;
				checkSum += g_data2[pos] >> 8;
			}
			
			for (i = 0; i < 10000; i++); //more then 2ms

		}
	}
	PORTC = 0b0011;
	//if (1 == GetCommandI2C(COMMAND_FLASH_START))
	//	PORTC = 0b1111;

	if (checkSum == GetCommandI2C(COMMAND_FLASH_CHECKSUM))
		PORTC = 0b0111;
	PutCommandI2C(COMMAND_FLASH_END, NULL, 0);
	while(1)
	{}
}