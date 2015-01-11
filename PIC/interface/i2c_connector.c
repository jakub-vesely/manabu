#include <i2c.h>
#include <p18F14K50.h> 


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

	SSPADD = (FREQ/(BITRATE*4))-1;
	OpenI2C(MASTER, SLEW_OFF);  
}

void PutI2C(int address, int data)
{
	StartI2C(); 
	while (WriteI2C(address << 1) != 0);
	while (WriteI2C(data) != 0);
	StopI2C();
}

unsigned GetI2C(int address)
{
	unsigned value;
	StartI2C(); 
	while (WriteI2C((address << 1) + 1) != 0);
	value = ReadI2C();
	StopI2C();
	return value;
}

unsigned char FillFromI2C(int address, char *data)
{
	unsigned char size;
	int i;
		
	size = GetI2C(address);
	data[0] = size;
	
	for (i = 1;i< size; i++)
		data[i] = GetI2C(address);

	return size;
}