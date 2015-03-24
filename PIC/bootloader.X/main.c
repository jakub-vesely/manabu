#include <common/i2c.h>

#define IS_DATA SSPSTATbits.D_nA

unsigned char sizeSet = 0;
unsigned size;  
unsigned counter = 0;

void ReadI2C()
{
	if (!SSP1IF) //MSSP interupt flag (SPI or I2C)
		return;

	SSP1IF = 0;
	
	if (0 == sizeSet)
	{
		if (SSPSTATbits.D_nA)
			size = (size << 8) + SSPBUF;
		else
			size = SSPBUF;

		if (counter == size)
		{
			//finished
		}

		//write data
	}
	if (SEN)
		CKP = 1;	
}

int main()
{
	I2cSlaveInit();
	while (1)
	{
		
	}
}

