#include <xc.h>
#include <pic16f1503.h>
#include <common/i2c.h>
#include <common/common.h>
#include <common/common_16F1503.h>
#include <common/Flash.h>
__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

#define FLASH_VERSION 0
void unlock (void)
{
#asm
	BANKSEL PMCON2
	MOVLW 0x55
	MOVWF PMCON2 & 0x7F
	MOVLW 0xAA
	MOVWF PMCON2 & 0x7F
	BSF PMCON1 & 0x7F,1 ; set WR bit
	NOP
	NOP
#endasm
}

#define FLASH_WRITE(address, data, latch)
	PMADR = address; \
	PMDAT = data; \
	PMCON1bits.LWLO = latch; /* 1 = latch, 0 = write row*/ \
	PMCON1bits.CFGS = 0; /* select the Flash address space*/ \
	PMCON1bits.FREE = 0; /* next operation will be a write*/ \
	PMCON1bits.WREN = 1; /* enable Flash memory write/erase*/ \
	unlock();

#define FLASH_ERASE(address) \
	PMADR = address; \
	PMCON1bits.CFGS = 0; /* select the Flash address space */ \
	PMCON1bits.FREE = 1; /* next operation will be an erase*/ \
	PMCON1bits.WREN = 1; /* enable Flash memory write/erase*/ \
	unlock(); \
	PMCON1bits.WREN = 0; /* disable Flash memory write/erase*/


#define RUN_PROGRAM_FLAG_POSITION HEFLASH_START

void main(void)
{
	//INTCONbits.GIE = 0;
	//TRISC5 = 0;
	//PORTCbits.RC5 = 0;

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
		
			if (0xf == SSPBUF)
			{
					SSPBUF = FLASH_VERSION;
			}
			else
			{
				FLASH_ERASE(RUN_PROGRAM_FLAG_POSITION);
				FLASH_WRITE(RUN_PROGRAM_FLAG_POSITION, SSPBUF, 0);
				//PORTCbits.RC5 = 1;
			}

			CKP = 1;
		}


		/*int i = 2000;
		
		while( i-- > 0);

		PORTCbits.RC5 = 0;
		i = 2000;
		while( i-- > 0);
		*/
	}
}