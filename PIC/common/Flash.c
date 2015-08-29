/*copied from http://ww1.microchip.com/downloads/en/AppNotes/00001673A.pdf*/
/*changed to be a little bit more memory effective*/
/*
* File: Flash.c
*
* Self-Write Flash support functions
*
* Author: Lucio Di Jasio
*
* Created on August 28, 2013
*/
#include "Flash.h"
/******************************************************************************
* Generic Flash functions
*/
unsigned FLASH_readConfig (unsigned address)
{
	// 1. load the address pointers
	PMADR = address;
	PMCON1bits.CFGS = 1; //select the configuration Flash address space
	PMCON1bits.RD = 1; //next operation will be a read
	NOP();
	NOP();
	// 2. return value read
	return PMDAT;
}//FLASH_config

void FLASH_readBlock (unsigned *buffer, unsigned address, char count)
{
	while (count > 0)
	{
		*buffer++ = FLASH_READ_BYTE (address++);
		count--;
	}
}//FLASH_readBLock

/**
* unlock Flash Sequence
*/
void _unlock (void)
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
}//unlock
