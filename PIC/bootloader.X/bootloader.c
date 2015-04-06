#include <xc.h>
#include <pic16f1503.h>
#include <common/i2c.h>
#include <common/common_16F1503.h>
#include <common/Flash.h>
#include <stdbool.h>
#include <CommonConstants.h>

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

#define IS_DATA SSPSTATbits.D_nA

#ifdef INTERUPT_REDIRECION
void interrupt serrvice_isr()
{
	#asm
		GOTO 0x204;
	#endasm
}
#endif

unsigned g_flashAddr;
unsigned char g_command = 0;
unsigned g_word;

struct g_bitFiled
{
	bool wordReady:1;
	bool lo:1;
} g_bitFiled;

#define FLASH_READ(address) \
	PMADR = address; \
	PMCON1bits.CFGS = 0; /*select the Flash address space*/ \
	PMCON1bits.RD = 1; /*next operation will be a read*/ \
	NOP(); \
	NOP();

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

#define FLASH_WRITE(address, data, latch) \
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

void ReadI2C()
{
	if (!SSP1IF) //MSSP interupt flag (SPI or I2C)
		return;

	SSP1IF = 0;

	if (!IS_DATA)
		g_command = SSPBUF >> 2;
	else
	{
		if (COMMAND_FLASH_ADDRESS == g_command)
		{
			if (g_bitFiled.lo)
				g_flashAddr = SSPBUF;
			else
			{
				g_flashAddr |= (SSPBUF << 8);
				FLASH_ERASE(g_flashAddr);
			}
			g_bitFiled.lo != g_bitFiled.lo;

		}
		else if (
			COMMAND_FLASH_LATCH_WORD == g_command ||
			COMMAND_FLASH_WRITE_WORD == g_command)
		{
			if (g_bitFiled.lo)
				g_word = SSPBUF;
			else
			{
				g_word |= (SSPBUF << 8);
				g_bitFiled.wordReady = true;
			}
			g_bitFiled.lo != g_bitFiled.lo;
		}
	}
	if (SEN)
		CKP = 1;
}

int main()
{
	//OSCCONbits.IRCF = 0b1111; //16MHz
	//while (!OSCSTATbits.HFIOFS);

	g_bitFiled.wordReady = false;
	g_bitFiled.lo = true;
	ANSELC = 0; //no analog pins

	//swich is set to input
	INnOUT_TRIS = 0;
	INnOUT_PORT = 1;

	I2C_COMMON_INIT
	I2C_SLAVE_SPECIFIC_INIT

	while (1)
	{
		if (g_command == COMMAND_FLASH_END)
		{
			FLASH_ERASE(0x7f0);
			FLASH_WRITE(0x7f0, 0x1234, 0);
		}

		FLASH_READ(0x7f0);
		if(0x1234 == PMDAT)
		{
#asm
			goto 0x200;
#endasm
		}
			
		ReadI2C();

		if (g_bitFiled.wordReady)
		{
			g_bitFiled.wordReady = false;
			FLASH_WRITE(g_flashAddr, g_word, COMMAND_FLASH_LATCH_WORD == g_command);
			g_flashAddr++;
		}
	}
}

