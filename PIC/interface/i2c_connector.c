#include "i2c_connector.h"

#include <i2c.h>
#include <p18F14K50.h>

#define SEND_TRY  100
#define FREQ 48000000
#define BITRATE 100000

#define TRIS_SCL TRISBbits.TRISB6 
#define TRIS_SDA TRISBbits.TRISB4

#define PORT_SCL PORTBbits.RB6 
#define PORT_SDA PORTBbits.RB4 

#define LEDS PORTC

void I2CInit(void)
{
	OSCCONbits.IRCF = 7;
	ANSELHbits.ANS10 = 0;
	
	LATB=0;
	TRIS_SCL = 0;
	TRIS_SDA = 0;

	LATC=0;
	TRISC = 0;

	SSPADD = (FREQ/(BITRATE*4))-1;
	OpenI2C(MASTER, SLEW_OFF);  
}

unsigned char PutStateI2C(unsigned char state)
{
	return PutI2C(I2C_MESSAGE_TYPE_DATA, 0,  &state, 1);
}

unsigned char PutCommandI2C(I2cCommand command, unsigned char const *data, unsigned char count)
{
	return PutI2C(I2C_MESSAGE_TYPE_COMMAND, command, data, count);
}

unsigned char GetCommandI2C(I2cCommand command)
{
	return GetI2C(I2C_MESSAGE_TYPE_COMMAND, command, 0, 0);
}


unsigned char PutI2C(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	unsigned char i = 0;
	unsigned char j = 0;
	IdleI2C();
	for (; j < SEND_TRY; j++)
	{
		StartI2C();

		/*
		 * because I will communicate always with one slave only I dont need send an
		 * address so I will use this required byte for a message type
		 * on the slave I will mask all the address byte out
		 */
		if (WriteI2C((command << 2) | (messageType << 1)) != 0) //lowest bite is read/write (write = 0)
		{
			StopI2C();
			continue;
		}

		for (; i < count; i++)
		{
			if(WriteI2C(data[i]) != 0)
			{
				StopI2C();
				goto continue2;
			}
		}
		break;
continue2:
;
	}
	StopI2C();

	//FIXME: doesnt work, when slave is not connected WriteI2C finish correctly
	return 0;//(j == SEND_TRY);
}

//FIXME: I will nedd transfer value too because I ill have to specifie a modulle address
unsigned char GetI2C(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	unsigned char value;

	IdleI2C();
	StartI2C();
	IdleI2C();
	//for an explanation take alook to PutI2C
	PORTC = 0b1000;
	while (WriteI2C((command << 2) | (messageType << 1) | 1) != 0); //lowest bite is read/write (write = 0)
	IdleI2C();
	PORTC = 0b0100;
	value = ReadI2C();
	PORTC = 0b1100;
	IdleI2C();
	StopI2C();
	return value;
}