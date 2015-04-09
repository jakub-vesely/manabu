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
	unsigned char retVal;
	
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
			if (!GetCommandI2C(COMMAND_GET_CURRENT_MODE, &retVal))
				retVal = 0xff;
			ResponseChar(retVal);
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

 unsigned const g_data2[72] =
 {
0x01f0, 0x01f1, 0x01f2, 0x01f3, 0x01f4, 0x01f5, 0x2145, 0x00f9,
0x107e, 0x0020, 0x290b, 0x30ff, 0x00f7, 0x300f, 0x00f8, 0x0021,
0x128e, 0x0020, 0x128e, 0x3001, 0x02f7, 0x3000, 0x3bf8, 0x0878,
0x0477, 0x1d03, 0x2913, 0x0024, 0x1295, 0x0020, 0x100e, 0x108e,
0x0021, 0x140e, 0x148e, 0x3001, 0x0024, 0x0096, 0x0197, 0x0193,
0x0192, 0x0194, 0x3016, 0x0095, 0x1695, 0x0020, 0x168e, 0x0020,
0x1d91, 0x292f, 0x1191, 0x0024, 0x0811, 0x00f6, 0x3002, 0x36f6,
0x0b89, 0x2937, 0x0876, 0x3a03, 0x1d03, 0x292f, 0x0191, 0x1615,
0x0020, 0x1d91, 0x2940, 0x0001, 0x292f, 0x3401, 0x3fff, 0x3fff,
 };
 
void main()
{
	unsigned address = 0x100;
	unsigned i = 20000;
	unsigned pos = 0;
	unsigned count = 
	/*(PORTAbits.RA3 == 0) ? sizeof(g_data1)/sizeof(unsigned) :*/ sizeof(g_data2)/sizeof(unsigned);
	unsigned char checkSum = 0;
	unsigned char slaveChecksum;
	unsigned char version;

	LATC=0;
	TRISC = 0;
	I2CInit();
	IdleI2C();

	while (PORTAbits.RA3 != 0);

	PORTC = 0b0001;
	while (0 == GetCommandI2C(COMMAND_FLASH_START, &version));
	if (version == 0) //from main program. device will restart
	{
		unsigned i = 0;
		unsigned j = 0;
		
		PutCommandI2C(COMMAND_FLASH_START, 0, 0);
		PORTC = 0b0011;

		PORTC = 0b1111;
		//for (i = 0; i < 0xfff; i++)
		//	for (j = 0; j < 100; j++);
		PORTC = 0b1000;
		CloseI2C();
		IdleI2C();
		I2CInit();
		while (0 == GetCommandI2C(COMMAND_FLASH_START, &version));
		PORTC = 0b1100;
		if (version == 1) //from main program. device will restart
		{
			PORTC = 0b1110;
		}
	}
	/*//PORTC = 0b0011;
	for (;pos < count; pos++)
	{
		if (!(pos % 16))
		{
			PutCommandI2C(COMMAND_FLASH_ADDRESS, (unsigned char const *)&address, 2);
			checkSum += address & 0xff;
			checkSum += address >> 8;
			address += 16;
		}
		if (pos != count-1 && ((pos+ 1) % 16))
		{
			PutCommandI2C(COMMAND_FLASH_LATCH_WORD, &(g_data2[pos]), 2);
			checkSum += g_data2[pos] & 0xff;
			checkSum += g_data2[pos] >> 8;
		}
		else
		{
			
			PutCommandI2C(COMMAND_FLASH_WRITE_WORD, &(g_data2[pos]), 2);
			checkSum += g_data2[pos] & 0xff;
			checkSum += g_data2[pos] >> 8;
			
			for (i = 0; i < 10000; i++); //more then 2ms

		}
	}
	//PORTC = 0b0011;
	//if (1 == GetCommandI2C(COMMAND_FLASH_START))
	//	PORTC = 0b1111;

	GetCommandI2C(COMMAND_FLASH_CHECKSUM, &slaveChecksum);
	//if (checkSum == slaveChecksum)
	//	PORTC = 0b0111;
	PutCommandI2C(COMMAND_FLASH_END, NULL, 0);
	 */
	while(1)
	{}
}