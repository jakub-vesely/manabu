#include <i2c.h>
#include <p18F14K50.h>

#include "i2c_connector.h"


#define FREQ 16000000
#define BITRATE 100000 

#define TRIS_SCL TRISBbits.TRISB6 
#define TRIS_SDA TRISBbits.TRISB4

#define PORT_SCL PORTBbits.RB6 
#define PORT_SDA PORTBbits.RB4 

#define LEDS PORTC

void I2CInit(void)
{
	unsigned value;
	OSCCONbits.IRCF = 7;
	ANSELH = 0x0;
	ANSEL = 0x0;
	ANSELHbits.ANS10 = 0;
	
	LATB=0;
	TRIS_SCL = 0;
	TRIS_SDA = 0;

	LATC=0;
	TRISC = 0;

	SSPADD = (FREQ/(BITRATE*4))-1; //TODO: 16F1503 datacheet, page 211
	OpenI2C(MASTER, SLEW_OFF);  
}

void PutI2C(unsigned char address, unsigned char *data, unsigned char count, unsigned char sendStop)
{
	//TODO: I cound check current address and send address message just in the case the address is different
	unsigned char i = 0;
	IdleI2C();
	StartI2C();
	IdleI2C();
	while (WriteI2C(address << 1) != 0);
	for (; i < count; i++)
	{
		IdleI2C();
		while (WriteI2C(data[i]) != 0);
	}
	IdleI2C();
	if (sendStop)
		StopI2C();
}

unsigned GetI2C(int address, unsigned char sendStart)
{
	unsigned value;
	IdleI2C();
	if (sendStart)
		StartI2C();
	IdleI2C();
	while (WriteI2C((address << 1) + 1) != 0);
	IdleI2C();
	value = ReadI2C();
	IdleI2C();
	StopI2C();
	return value;
}

unsigned char PutAndGetI2C(unsigned char address, unsigned char *data, unsigned char count)
{
	PutI2C(address, data, count, 0);
	RestartI2C();
	return GetI2C(address, 0);
}

/*unsigned char FillFromI2C(int address, char *data)
{
	unsigned char size;
	int i;
		
	size = GetI2C(address, 1);
	data[0] = size;
	
	for (i = 1;i< size; i++)
		data[i] = GetI2C(address, 1);

	return size;
}*/