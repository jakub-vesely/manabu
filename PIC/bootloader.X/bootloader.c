#include <xc.h>
#include <pic16f1503.h>
//#include <common/i2c.h>

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
void interrupt serrvice_isr()
{
	#asm
		GOTO 0x204;
	#endasm
}

int main()
{
	/*#asm
		goto 0x200;
	#endasm
	//I2cSlaveInit();
	while (1)
	{
		
	}*/

	LATC5 = 0;
	TRISC5 = 0;
	
	while(1)
	{
		PORTCbits.RC5 = 1;

	}
 
}

