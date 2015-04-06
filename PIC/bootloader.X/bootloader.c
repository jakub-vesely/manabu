#include <xc.h>
#include <pic16f1503.h>
//#include <common/i2c.h>
#include <common/Flash.h>
__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

/*#define IS_DATA SSPSTATbits.D_nA

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
*/
#ifdef INTERUPT_REDIRECION
void interrupt serrvice_isr()
{
	#asm
		GOTO 0x204;
	#endasm
}
#endif

int main()
{
	unsigned const data[8] = {0x107e, 0x0020, 0x2a03, 0x0021, 0x128e, 0x0020, 0x168e , 0x2a07};
	char i = 0;
	unsigned add = 0x200;

	unsigned buffer[8];
	I2cSlaveInit();
	FLASH_erase (add);

	for (; i < 7; i++)
		FLASH_write(add+i, data[i], 1);
    FLASH_write(add+i, data[i], 0);

	#asm
		goto 0x200;
	#endasm
	while (1)
	{
		
	}
}

