#ifndef _COMMON_FLASH_H_
#define _COMMON_FLASH_H_

/*copied from http://ww1.microchip.com/downloads/en/AppNotes/00001673A.pdf*/
/*
* Flash.h
*
*/
#include<xc.h>
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
unsigned FLASH_read (unsigned address);
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
void FLASH_write (unsigned address, unsigned data, char latch);
/**
* Erase a row of Flash memory
*
* @param address absolute address in Flash contained in selected row
*/
void FLASH_erase (unsigned address);

#endif //_COMMON_FLASH_H_