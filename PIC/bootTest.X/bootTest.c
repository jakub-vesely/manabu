#include <xc.h>
#include <pic16f1503.h>
#include <common/i2c.h>
#include <common/common.h>
#include <system_common.h>
#include <common/Flash.h>

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

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
				if (MID_COMMAND_FLASH_GET_VERSION == (value >> 2))
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