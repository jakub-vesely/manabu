#include <xc.h>
#include <pic16f1503.h>
#include <common/i2c.h>
#include <common/common_16F1503.h>
#include <common/Flash.h>
#include <stdbool.h>
#include <CommonConstants.h>

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

#define VERSION 1
#define MAIN_PROOGRAM_START 0x100
#define RUN_PROGRAM_FLAG_POSITION HEFLASH_START
#define RUN_PROGRAM_VALUE 0
#define IS_DATA SSPSTATbits.D_nA
#define IS_READ  SSPSTATbits.R_nW

unsigned g_flashAddr;
unsigned char g_command = 0;
unsigned g_word;
unsigned checkSum = 0;
struct g_bitFiled
{
	bool dataReady:1;
	bool lo:1;
} g_bitFiled;

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

#define FLASH_READ_BYTE(address) \
	PMADR = address; \
	PMCON1bits.CFGS = 0; /*select the Flash address space*/ \
	PMCON1bits.RD = 1; /*next operation will be a read*/ \
	NOP(); \
	NOP();

void ReadI2C()
{
	if (!SSP1IF) //MSSP interupt flag (SPI or I2C)
		return;

	SSP1IF = 0;

	unsigned char value = SSPBUF;

	if (!IS_DATA)
	{
		g_command = value >> 2;

		if (COMMAND_FLASH_START == g_command)
		{
			SSPBUF = VERSION;
		}
		else
		if (COMMAND_FLASH_CHECKSUM == g_command)
			SSPBUF = checkSum;
	}
	else
	{
		checkSum += value;

		if (g_bitFiled.lo)
		{
			g_bitFiled.lo = false;
			g_word = value;
		}
		else
		{
			g_bitFiled.lo = true;
			g_word |= (value << 8);

			if (COMMAND_FLASH_ADDRESS == g_command)
			{
				g_flashAddr = g_word;
				FLASH_ERASE(g_flashAddr);
			}
			else /*COMMAND_FLASH_LATCH_WORD == g_command ||	COMMAND_FLASH_WRITE_WORD == g_command*/
				g_bitFiled.dataReady = true;
		}
	}
	if (SEN)
		CKP = 1;
}

void interrupt serrvice_isr()
{
	#asm
		GOTO (MAIN_PROOGRAM_START + 4);
	#endasm
}

int main()
{
	INTCONbits.GIE = 0;
	//OSCCONbits.IRCF = 0b1111; //16MHz
	//while (!OSCSTATbits.HFIOFS);

	FLASH_READ_BYTE(RUN_PROGRAM_FLAG_POSITION)
	if (RUN_PROGRAM_VALUE == PMDAT)
	{
#asm
		GOTO MAIN_PROOGRAM_START;
#endasm
	}

	g_bitFiled.dataReady = false;
	g_bitFiled.lo = true;
	ANSELC = 0; //no analog pins

	//swich is set to input
	INnOUT_TRIS = 0;
	INnOUT_PORT = 1;

	I2C_COMMON_INIT
	I2C_SLAVE_SPECIFIC_INIT

	while (1)
	{
		if(g_command == COMMAND_FLASH_END)
		{
#asm
			GOTO MAIN_PROOGRAM_START;
#endasm
		}

		ReadI2C();

		if (g_bitFiled.dataReady)
		{
			g_bitFiled.dataReady = false;
			FLASH_WRITE(g_flashAddr, g_word, COMMAND_FLASH_LATCH_WORD == g_command);
			checkSum += PMCON1bits.WRERR; //0 write success, 1 write error
			g_flashAddr++;
		}
	}
}

