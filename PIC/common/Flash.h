#ifndef _COMMON_FLASH_H_
#define _COMMON_FLASH_H_

/*copied from http://ww1.microchip.com/downloads/en/AppNotes/00001673A.pdf*/
/*changed to be a little bit more memory effective*/
/*
* Flash.h
*
*/
#include<xc.h>
#include <system_common.h>

#if defined(_PIC16F1501_H_)
//1K
#define FLASH_ROWSIZE 16 //size of a row
#define HEFLASH_START 0x0380 //first address in HE Flash memory
#define HEFLASH_END 0x03FF //last address in HE Flash memory
#elif defined(_PIC16F1503_H_)||defined(_PIC16F1507_H_)||defined(_PIC16F1512_H_)||\
defined(_PIC16F1703_H_)||defined(_PIC16F1707_H_)
//2K
#define FLASH_ROWSIZE 16 //size of a row
#define HEFLASH_START 0x0780 //first address in HE Flash memory
#define HEFLASH_END 0x07FF //last address in HE Flash memory
#elif defined(_PIC16F1508_H_)||defined(_PIC16F1513_H_)||\
defined(_PIC16F1704_H_)||defined(_PIC16F1708_H_)||defined(_PIC16F1713_H_)
//4K
#define FLASH_ROWSIZE 32 //size of a row
#define HEFLASH_START 0x0F80 //first address in HE Flash memory
#define HEFLASH_END 0x0FFF //last address in HE Flash memory
#elif defined(_PIC16F1509_H_)||defined(_PIC16F1526_H_)||\
defined(_PIC16F1454_H_)||defined(_PIC16F1455_H_)||defined(_PIC16F1459_H_)||\
defined(_PIC16F1705_H_)||defined(_PIC16F1709_H_)||\
defined(_PIC16F1716_H_)||defined(_PIC16F1717_H_)
//8K
#define FLASH_ROWSIZE 32 //size of a row
#define HEFLASH_START 0x1F80 //first address in HE Flash memory
#define HEFLASH_END 0x1FFF //last address in HE Flash memory
#elif defined(_PIC16F1518_H)||defined(_PIC16F1519_H)||defined(_PIC16F1527_H_)||\
defined(_PIC16F1718_H_)||defined(_PIC16F1719_H_)
//16K
#define FLASH_ROWSIZE 32 //size of a row
#define HEFLASH_START 0x3F80 //first address in HE Flash memory
#define HEFLASH_END 0x3FFF //last address in HE Flash memory
#endif
#define FLASH_ROWMASK FLASH_ROWSIZE-1
/******************************************************************************
* Generic Flash functions
*/
/**
* Read a word from program Flash memory
*
* @param address source address (absolute Flash memory address)
* @return word retrieved from Flash memory
*/
#define FLASH_READ_BYTE(address) \
    PMADR = address; \
    PMCON1bits.CFGS = 0; /*select the Flash address space*/ \
    PMCON1bits.RD = 1; /*next operation will be a read*/ \
    NOP(); \
    NOP();

/**
* Read a word from configuration Flash memory
*
* @param address source address (absolute Flash memory address)
* @return word retrieved from Flash memory
*/
unsigned FLASH_readConfig (unsigned address);
/**
* Read a block of words from program Flash memory
*
* @param buffer destination buffer (must be sufficiently large)
* @param address source address (absolute Flash memory address)
* @param count number of words to be retrieved
*/
void FLASH_readBlock (unsigned* buffer, unsigned address, char count);
/**
* Write a word of data to Flash memory (latches)
* an actual write is performed only if LWLO = 0, data is latched if LWLO = 1
*
* @param address destination address (absolute Flash memory)
* @param data word of data to be written (latched)
* @param latch 1 = latch, 0 = write
*/
#define FLASH_WRITE_BODY(address, data, latch) \
    PMADR = address; \
    PMDAT = data; \
    PMCON1bits.LWLO = latch; /* 1 = latch, 0 = write row*/ \
    PMCON1bits.CFGS = 0; /* select the Flash address space*/ \
    PMCON1bits.FREE = 0; /* next operation will be a write*/ \
    PMCON1bits.WREN = 1; /* enable Flash memory write/erase*/ \
    _unlock();

#ifndef INTERUPTS_DISABLED
#define FLASH_WRITE(address, data, latch) \
	char temp = INTCONbits.GIE; \
	INTCONbits.GIE = 0; \
	FLASH_WRITE_BODY(address, data, latch) \
	if (temp) \
		INTCONbits.GIE = 1;
#else
#define FLASH_WRITE(address, data, latch)\
	FLASH_WRITE_BODY(address, data, latch)
#endif

/**
* Erase a row of Flash memory
*
* @param address absolute address in Flash contained in selected row
*/
#define FLASH_ERASE_BODY(address) \
    PMADR = address; \
    PMCON1bits.CFGS = 0; /* select the Flash address space */ \
    PMCON1bits.FREE = 1; /* next operation will be an erase*/ \
    PMCON1bits.WREN = 1; /* enable Flash memory write/erase*/ \
    _unlock(); \
    PMCON1bits.WREN = 0; /* disable Flash memory write/erase*/

#ifndef INTERUPTS_DISABLED
#   define FLASH_ERASE(address) \
	char temp = INTCONbits.GIE; \
	INTCONbits.GIE = 0; \
	FLASH_ERASE_BODY(address) \
	if (temp) \
            INTCONbits.GIE = 1;
#else
#define FLASH_ERASE(address)\
    FLASH_ERASE_BODY(address)
#endif

#endif //_COMMON_FLASH_H_