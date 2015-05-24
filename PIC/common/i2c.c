#include <common/i2c.h>
#include <system_common.h>

#if defined(_PIC18F14K50_H_)
#include <i2c.h>
#endif

unsigned char g_stateFollowed = 0;

#define IS_DATA SSPSTATbits.D_nA
#define IS_READ  SSPSTATbits.R_nW

#define SEND_TRY_COUNT  5000
#define BITRATE 100000

unsigned char  g_bootloaderPolicy = 0;
void I2cSlaveInit()
{
#ifndef _PIC18F14K50_H_
	I2C_COMMON_INIT
	/*SSP1STAT &= 0x3F;                // power on state
  SSP1CON1 = 0x00;                 // power on state
  SSP1CON2 = 0x00;                 // power on state
  SSP1CON1 |= 0b00000110;           // select serial mode
  SSP1STAT |= 0b10000000;                // slew rate on/off

  SSP1CON1 |= 0b00100000;              // enable synchronous serial port
	//I2C_SLAVE_SPECIFIC_INIT
  */

	SSP1STAT &= 0x3F;                // power on state
  SSP1CON1 = 0x00;                 // power on state
  SSP1CON2 = 0x00;

	SSPCON2 = 0b00000001; /*SEN is set to enable clock stretching*/
    SSPCON3 = 0x00; 
    SSPMSK = 0x00; /*all address bits will be ignored*/
    SSPADD = 0x00; /*no address is set, all the time is conneceted only one slave*/
    SSPCON1 = 0b00010110; /*clock stretching + 7-bit addressing*/ 
    SSPEN = 1;
#endif
}

void I2cMasterInit(void)
{
#if defined(_PIC18F14K50_H_)
	OSCCONbits.IRCF = 7;
	ANSELHbits.ANS10 = 0;

	LATB=0;
	TRIS_SCL = 0;
	TRIS_SDA = 0;

	SSPADD = (FREQ/(BITRATE*4))-1;
	OpenI2C(MASTER, SLEW_OFF);
#else
	I2C_COMMON_INIT

	//I2C master mode setting
	SDAHT = 1;			//Minimum of 300 ns hold time on SDAx after the falling edge of SCLx
	SMP = 1;           //Slew rate control disabled for standard speed mode (100 kHz and 1 MHz)
	CKE = 1;           //Transmit occurs on transition from active to Idle clock state

	SSP1STAT &= 0x3F;                // power on state
	SSP1CON1 = 0x00;                 // power on state
	SSP1CON2 = 0x00;                 // power on state
	SSPMSK = 0;
	SSPCON3 = 0;
	SSP1STAT |= 0b10000000;
	
	//uncomment when slave will be process it
	//ACKEN = 1; //Acknowledge enabled

	SSPADD = (FREQ/(BITRATE*4))-1;
	ENABLE_I2C_MASTER_MODE_bit = 1;         //Enable I2C Master mode
	SSPEN = 1;         //Enable SSP module - I2C Initialized
#endif
 }

bool I2cMasterIdle(void)
{
#if defined(_PIC18F14K50_H_)
	IdleI2C();
	return true;
#else
	//unsigned counter = SEND_TRY_COUNT;
	while ((SSPCON2 & 0x1F) | (SSPSTATbits.R_nW))
	{
		//if (0 == counter--)
		//	return false;
	};
	return true;
#endif
	
}

void I2cMasterStart(void)
{
#if defined(_PIC18F14K50_H_)
	StartI2C();
#else
	/*SSPMSK = 0;
	SSPCON2 = 0;
	SSPCON3 = 0;
	SSPSTAT = 0;
	*/
	SSPCON2bits.SEN=1;
	while(SSPCON2bits.SEN)
  {} //SEM will be cleared automaticlay after seting of start condition on a bus
#endif
}
/*
FIXME: legacy - returns false when success
*/
bool I2cMasterWrite(char byte)
{
#if defined(_PIC18F14K50_H_)
	return 0 == WriteI2C(byte);
#else
	SSPBUF = byte;
	while(SSP1STATbits.R_nW)
	{} //R_nW will be cleared automatticly after whole message is written on a bus
	return 0 == SSPCON2bits.ACKSTAT;
#endif
}

bool I2cMasterRead(unsigned char *retVal)
{
#if defined(_PIC18F14K50_H_)
	*retVal = ReadI2C();
	return true;
#else
  
	unsigned counter = SEND_TRY_COUNT;
	
	if (!I2cMasterIdle())
	 return false;
	 
	SSPCON2bits.RCEN = 1;
	while(SSPCON2bits.RCEN) //FIXME: may be ther should be !BF
	{
		if (0 == counter-- || INPUT_MESSAGE_MISSED) //timeout or predessor try to send a message to me
			return false;
	}
	*retVal = SSPBUF;
	
  //uncomment when slave will be process it
  //ACKDT = 0;
  return true;
#endif
}

void I2cMasterStop(void)
{
#if defined(_PIC18F14K50_H_)
	StopI2C();
#else
	SSPCON2bits.PEN = 1;
	while(SSPCON2bits.PEN)
	{} //PEN is cleared automaticaly when stop combination is send  
#endif
}

bool I2cMasterPut(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	unsigned char i = 0;
	
	//if (!I2cMasterIdle())
	//	return false;
	
	I2cMasterStart();
	if(!INPUT_MESSAGE_MISSED)
	{
		/*
		 * because I will communicate always with one slave only I dont need send an
		 * address so I will use this required byte for a message type
		 * on the slave I will mask all the address byte out
		 */
		if (!I2cMasterWrite((command << 2) | (messageType << 1))) //lowest bite is read/write (write = 0)
		{
			I2cMasterStop();
			return false;
		}

		for (; i < count; i++)
		{
			if(!I2cMasterWrite(data[i]))
			{
				I2cMasterStop();
				return false;
			}
		}
	}
	I2cMasterStop();
	return true;
}

bool I2cMasterGet(unsigned char messageType, I2cCommand command, unsigned char *retVal)
{

	I2cMasterStart();
	
	if (!I2cMasterWrite((command << 2) | (messageType << 1) | 1))
	{
		I2cMasterStop();
		return false;
	}

	if (!I2cMasterRead(retVal))
	{
		I2cMasterStop();
		return false;
	}
	I2cMasterStop();
	return true;
}

bool PutStateI2C(unsigned char state)
{
	return I2cMasterPut(I2C_MESSAGE_TYPE_DATA, 0,  &state, 1);
}

bool PutCommandI2C(I2cCommand command, unsigned char const *data, unsigned char count)
{
	return I2cMasterPut(I2C_MESSAGE_TYPE_COMMAND, command, data, count);
}

bool GetCommandI2C(I2cCommand command, unsigned char *retVal)
{
	return I2cMasterGet(I2C_MESSAGE_TYPE_COMMAND, command, retVal);
}

bool CheckI2cAsSlave(void)
{
#ifndef _PIC18F14K50_H_

	unsigned char value;
	if (!SSP1IF) //MSSP interupt flag (SPI or I2C)
		return false;

	SSP1IF = 0;
	value = SSPBUF;

	//FIXME: I should wait for processing last command or data
	if (!IS_DATA) //"address" byte in write mode
	{	
		g_stateFollowed = (0 == (value & 2)); //second lowest bite is I2C_MESSAGE_TYPE where 0 means data
		g_commandInstruction = (value >> 2);

		switch (g_commandInstruction)
		{
			case COMMAND_FLASH_GET_VERSION:
		
				g_bootloaderPolicy = 1;
				SSPBUF = 0; //I'm in program so i dont have a bootloader version
			break;
			case COMMAND_GET_CURRENT_MODE:
				SSPBUF = g_persistant.mode;
			break;
			case COMMAND_GET_STATE:
				SSPBUF = g_state;
			break;
		}
		
		//I want to be sure this insrtuction didn't come by a mistake
		//It is very important because after this isnstuction is chip set to
		//a bootloader state and the program is not accessible any more
		/*else if (COMMAND_FLASH_SET_BOOT_FLAG == g_commandInstruction &&
			0 == g_bootloaderPolicy)
		{
			//g_commandInstruction = COMMAND_NONE; //set boot flag sequence is not complete. I don't want to risk bootloader state
		}*/

		if (COMMAND_FLASH_GET_VERSION != g_commandInstruction)
			g_bootloaderPolicy = 0;
	}
	else
	{
		g_bootloaderPolicy = 0;
		if (g_stateFollowed)
		{
			g_state = value;
			g_stateChanged = true;
		}
		else
		{
			g_commandValue = value;
			g_commandRecieved = true;
		}
	}
	if (SEN)
		CKP = 1;

	return true;
#else
	return false;
#endif
}