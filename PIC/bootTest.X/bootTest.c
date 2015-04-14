#include <xc.h>
#include <pic16f1503.h>
#include <common/i2c.h>
#include <common/common.h>
#include <common/common_16F1503.h>
#include <common/Flash.h>

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

#define FLASH_VERSION 0
#define RUN_PROGRAM_FLAG_POSITION HEFLASH_START

void main(void)
{
	unsigned char value;

	INTCONbits.GIE = 0;
	TRISC5 = 0;
	PORTCbits.RC5 = 1;

	ANSELC = 0; //no analog pins
	//swich is set to input
	INnOUT_TRIS = 0;
	INnOUT_PORT = 1;
	
	I2C_COMMON_INIT
	I2C_SLAVE_SPECIFIC_INIT


	while(1)
	{
		if (SSP1IF) //MSSP interupt flag (SPI or I2C)
		{
			SSP1IF = 0;
			value = SSPBUF;
			if (!IS_DATA)
			{
				if (COMMAND_FLASH_GET_VERSION == (value >> 2))
						SSPBUF = FLASH_VERSION;
						
				CKP = 1;
			}
			else
			{
				CKP = 1;
				FLASH_ERASE(RUN_PROGRAM_FLAG_POSITION);
				FLASH_WRITE(RUN_PROGRAM_FLAG_POSITION, value, 0);

				if (0 != value)
				{
#asm
	RESET
#endasm
				}
			}
		}



		/*int i = 2000;
		
		while( i-- > 0);

		PORTCbits.RC5 = 0;
		i = 2000;
		while( i-- > 0);
		*/
	}
}