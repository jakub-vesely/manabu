#include "i2c_connector.h"

#include <i2c.h>
#include <p18F14K50.h>

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
	ANSELH = 0x0;
	ANSEL = 0x0;
	ANSELHbits.ANS10 = 0;
	
	LATB=0;
	TRIS_SCL = 0;
	TRIS_SDA = 0;

	LATC=0;
	TRISC = 0;

	SSPADD = (FREQ/(BITRATE*4))-1;
	OpenI2C(MASTER, SLEW_OFF);  
}

void PutStateI2C(unsigned char state)
{
	PutI2C(I2C_MESSAGE_TYPE_DATA, 0,  &state, 1);
}

void PutCommandI2C(I2cCommand command, unsigned char *data, unsigned char count)
{
	PutI2C(I2C_MESSAGE_TYPE_COMMAND, command, data, count);
}

unsigned char GetCommandI2C(I2cCommand command)
{
	return GetI2C(I2C_MESSAGE_TYPE_COMMAND, command, 0, 0);
}

void PutI2C(unsigned char messageType, I2cCommand command, unsigned char *data, unsigned char count)
{
	//FIXME: I guess I dont nedd and IdleI2C. check it!
	unsigned char i = 0;
	IdleI2C();
	StartI2C();
	IdleI2C();

	/*
	 * because I will communicate always with one slave only I dont need send an
	 * address so I will use this required byte for a message type
	 * on the slave I will mask oall the address byte out
	 */
	while (WriteI2C((command << 2) | (messageType << 1)) != 0); //lowest bite is read/write (write = 0)

	for (; i < count; i++)
	{
		IdleI2C();
		while (WriteI2C(data[i]) != 0);
	}
	IdleI2C();
	StopI2C();
}

//FIXME: I will nedd transfer value too because I ill have to specifie a modulle address
unsigned char GetI2C(unsigned char messageType, I2cCommand command, unsigned char *data, unsigned char count)
{
	unsigned char value;

	IdleI2C();
	StartI2C();
	IdleI2C();
	//for an explanation take alook to PutI2C
	while (WriteI2C((command << 2) | (messageType << 1) | 1) != 0); //lowest bite is read/write (write = 0)
	IdleI2C();
	value = ReadI2C();
	IdleI2C();
	StopI2C();
	return value;
}