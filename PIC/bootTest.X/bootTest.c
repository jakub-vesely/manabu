#include <xc.h>
#include <pic16f1503.h>
#include <common/i2c.h>
#include <common/common.h>
__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);


void main(void)
{
	unsigned timeout = 0xfff;
	TRISC5 = 0;
	PORTCbits.RC5 = 0;
	
	while (0 != --timeout);
	
	I2C_COMMON_INIT
	I2C_SLAVE_SPECIFIC_INIT


	PORTCbits.RC5 = 1;
	while(1)
	{
		if (SSP1IF) //MSSP interupt flag (SPI or I2C)
		{
			SSP1IF = 0;

			if (COMMAND_FLASH_START == (SSPBUF >> 2))
			{
				SSPBUF = 0;
				SSPCON1bits.CKP = 1;        // release clock line 
				while ( !PIR1bits.SSP1IF ); // wait until ninth clock pulse received
#asm
	RESET;
#endasm
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