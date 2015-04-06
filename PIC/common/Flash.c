/*copied from http://ww1.microchip.com/downloads/en/AppNotes/00001673A.pdf*/
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

unsigned FLASH_read (unsigned address)
{
	// 1. load the address pointers
	PMADR = address;
	PMCON1bits.CFGS = 0; //select the Flash address space
	PMCON1bits.RD = 1; //next operation will be a read
	NOP();
	NOP();
	// 2. return value read
	return PMDAT;
}//FLASH_read

void FLASH_readBlock (unsigned *buffer, unsigned address, char count)
{
	while (count > 0)
	{
		*buffer++ = FLASH_read (address++);
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

void FLASH_write (unsigned address, unsigned data, char latch)
{
#ifndef INTERUPTS_ENABLED
	// 1. disable interrupts (remember setting)
	char temp = INTCONbits.GIE;
	INTCONbits.GIE = 0;
#endif
	// 2. load the address pointers
	PMADR = address;
	PMDAT = data;
	PMCON1bits.LWLO = latch; // 1 = latch, 0 = write row
	PMCON1bits.CFGS = 0; // select the Flash address space
	PMCON1bits.FREE = 0; // next operation will be a write
	PMCON1bits.WREN = 1; // enable Flash memory write/erase
	// 3. perform unlock sequence
	_unlock();
	// 4. restore interrupts
#ifndef INTERUPTS_ENABLED
	if (temp)
		INTCONbits.GIE = 1;
#endif
}//FLASH_write

void FLASH_erase (unsigned address)
{
#ifndef INTERUPTS_ENABLED
	// 1. disable interrupts (remember setting)
	char temp = INTCONbits.GIE;
	INTCONbits.GIE = 0;
#endif
	// 2. load the address pointers
	PMADR = address;
	PMCON1bits.CFGS = 0; // select the Flash address space
	PMCON1bits.FREE = 1; // next operation will be an erase
	PMCON1bits.WREN = 1; // enable Flash memory write/erase
	// 3. perform unlock sequence and erase
	_unlock();
	// 4. disable writes and restore interrupts
	PMCON1bits.WREN = 0; // disable Flash memory write/erase

#ifndef INTERUPTS_ENABLED
	if (temp)
		INTCONbits.GIE = 1;
#endif
}//FLASH_erase