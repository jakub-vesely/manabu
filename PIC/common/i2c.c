#include <common/i2c.h>
#include <system_common.h>

#ifndef INTERFACE
#	include <moduleTypeSpecific.h>
#endif

#if defined(_PIC18F14K50_H_)
#include <i2c.h>
#endif

unsigned char g_inStateFollowed = 0;
unsigned char g_inStateHiTemp = 0;

#define IS_DATA SSPSTATbits.D_nA
#define IS_READ  SSPSTATbits.R_nW

#define SEND_TRY_COUNT  5000
#define BITRATE 100000

unsigned char  g_bootloaderPolicy = 0;
void I2cSlaveInit()
{
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
}

void I2cMasterInit(void)
{
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
 }

bool I2cMasterIdle(void)
{
	//unsigned counter = SEND_TRY_COUNT;
	while ((SSPCON2 & 0x1F) | (SSPSTATbits.R_nW))
	{
		//if (0 == counter--)
		//	return false;
	};
	return true;
}

void I2cMasterStart(void)
{
	/*SSPMSK = 0;
	SSPCON2 = 0;
	SSPCON3 = 0;
	SSPSTAT = 0;
	*/
	SSPCON2bits.SEN=1;
	while(SSPCON2bits.SEN)
  {} //SEM will be cleared automaticlay after seting of start condition on a bus
}
/*
FIXME: legacy - returns false when success
*/
bool I2cMasterWrite(char byte)
{
	SSPBUF = byte;
	while(SSP1STATbits.R_nW)
	{} //R_nW will be cleared automatticly after whole message is written on a bus
	return 0 == SSPCON2bits.ACKSTAT;
}

bool I2cMasterRead(unsigned char *retVal)
{
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
	return true;
}

void I2cMasterStop(void)
{
	SSPCON2bits.PEN = 1;
	while(SSPCON2bits.PEN)
	{} //PEN is cleared automaticaly when stop combination is send  
}

bool I2cMasterPut(unsigned char messageType, MessageId command, unsigned char const *data, unsigned char count)
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
		unsigned char mixture = 0;
		mixture |= (messageType << 1);
		if (I2C_MESSAGE_TYPE_COMMAND ==  messageType)
			mixture |= (command << 2);
		else
		{
				//state is 10 bits long and for high two bits is used first address byte
				mixture |= data[1] << 2;
				count--;
		}
		
			

		if (!I2cMasterWrite(mixture)) //lowest bite is read/write (write = 0)
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

void I2CMasterAck( void )
{
  SSPCON2bits.ACKDT = 0;           // set acknowledge bit state for ACK
  SSPCON2bits.ACKEN = 1;           // initiate bus acknowledge sequence
}

bool I2cMasterGet(unsigned char messageType, MessageId command, unsigned char *retVal, unsigned char count)
{
	I2cMasterInit();
	I2cMasterIdle();
	I2cMasterStart();
	if (!I2cMasterWrite((command << 2) | (messageType << 1) | 1))
	{
		I2cMasterStop();
		return false;
	}
	for (unsigned char i = 0; i < count; i++)
	{
		
		if (!I2cMasterRead(retVal+i))
		{
			I2cMasterStop();
			return false;
		}
		if (i != count-1)
			I2CMasterAck();
	}
	I2cMasterStop();
	return true;
}

bool PutStateI2C(unsigned char state)
{
	return I2cMasterPut(I2C_MESSAGE_TYPE_DATA, 0,  &state, 1);
}

bool PutCommandToI2C(MessageId command, unsigned char const *data, unsigned char count)
{
	return I2cMasterPut(I2C_MESSAGE_TYPE_COMMAND, command, data, count);
}

bool GetCommandFromI2C(MessageId command, unsigned char *retVal, unsigned char count)
{
	return I2cMasterGet(I2C_MESSAGE_TYPE_COMMAND, command, retVal, count);
}

bool CheckI2cAsSlave(void)
{
#ifndef INTERFACE

	unsigned char value;
	if (!SSP1IF) //MSSP interupt flag (SPI or I2C)
		return false;

	value = SSPBUF;

	SSP1IF = false;

	//FIXME: I should wait for processing last command or data
	if (!IS_DATA) //"address" byte in write mode
	{	
		g_inStateFollowed = (0 == (value & 2)); //second lowest bite is I2C_MESSAGE_TYPE where 0 means data
		if (g_inStateFollowed)
			g_inStateHiTemp = (value >> 2);
		else
			g_commandInstruction = (value >> 2);

		if (IS_READ) //it have not be here - just for sure
		{
			switch (g_commandInstruction)
			{
				case MID_COMMAND_FLASH_GET_VERSION:

					g_bootloaderPolicy = 1;
					SSPBUF = 0; //I'm in program so i dont have a bootloader version
				break;
				case MID_GET_STATE:
					SSPBUF = (unsigned char)(g_outState & 0xff);
					SSPCON1bits.CKP = 1;
					while (!PIR1bits.SSP1IF) //wait until ninth clock pulse received
					{}
					SSPBUF = (unsigned char)(g_outState >> 8);
				break;
				case MID_GET_MODULE_TYPE:
					SSPBUF = GetModuleType();
				break;
				case MID_GET_MODE:
					SSPBUF = g_persistant.mode;
				//TODO: when i will process get with parameter i should place here g_stateMessageEnabled = false;
			}
		}
		else
		{
			g_stateMessageEnabled = false; //Im expecting second part of message so state sending must wait now
		}


		if (MID_COMMAND_FLASH_GET_VERSION != g_commandInstruction)
			g_bootloaderPolicy = 0;
	}
	else
	{
		g_bootloaderPolicy = 0;
		if (g_inStateFollowed)
		{
			g_inState = g_inStateHiTemp << 8;
			g_inState |= value;
			g_stateChanged = true;
		}
		else
		{
			g_commandValue = value;
			g_commandRecieved = true;
		}

		g_stateMessageEnabled = true; //Second part of message recieved. I ddon't wait for anthitng and can switch to output
	}

	if (SEN)
		CKP = 1;

	return true;
#else
	return false;
#endif
}