#include "i2c.h"

unsigned char g_stateFollowed = 0;

#define TO_OUTPUT_MAX_TRAY 10
#define IS_DATA SSPSTATbits.D_nA
#define IS_READ  SSPSTATbits.R_nW

#define SEND_TRY  100

unsigned char  g_bootloaderPolicy = 0;
void I2cSlaveInit()
{
	I2C_COMMON_INIT
	I2C_SLAVE_SPECIFIC_INIT
}

void I2cMasterInit(void)
{
	I2C_COMMON_INIT

	//I2C master mode setting
	SDAHT = 1;			//Minimum of 300 ns hold time on SDAx after the falling edge of SCLx
	SMP = 1;           //Slew rate control disabled for standard speed mode (100 kHz and 1 MHz)
	CKE = 1;           //Transmit occurs on transition from active to Idle clock state

	SSPMSK = 0;
	SSPCON1 = 0;
	SSPCON2 = 0;
	SSPCON3 = 0;
	SSPSTAT = 0;

	SSPADD = 39;		//100KHz
	ENABLE_I2C_MASTER_MODE_bit = 1;         //Enable I2C Master mode
	SSPEN = 1;         //Enable SSP module - I2C Initialized
 }

bool I2cMasterStart(void)
{
#if defined(_PIC18F14K50_H_)
	StartI2C();
	return true;
#else
	/*SSPMSK = 0;
	SSPCON2 = 0;
	SSPCON3 = 0;
	SSPSTAT = 0;
	*/
	unsigned char counter = 0;
	SSPCON2bits.SEN=1;
	while(SSPCON2bits.SEN)
	{
		if (SEND_TRY == counter)
			return false;

		counter = counter + 1;
	}
	return true;
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
	unsigned char counter = 0;
	SSPBUF = byte;
	while(SSP1STATbits.R_nW)
	{
		if (SEND_TRY == counter)
			return false;
		
		counter = counter + 1;
	}
	return 0 == SSPCON2bits.ACKSTAT;
#endif
}

bool I2cMasterRead(unsigned char *retVal)
{
#if defined(_PIC18F14K50_H_)
	return ReadI2C();
#else
	unsigned char counter = 0;

	SSPCON2bits.RCEN = 1;
	while(SSPCON2bits.RCEN) //FIXME: may be ther should be !BF
	{
		if (255 == counter)
			return false;

		counter = counter + 1;
	}
	*retVal = SSPBUF;
	return true;
#endif
}

bool I2cMasterStop(void)
{
#if defined(_PIC18F14K50_H_)
	StopI2C();
	return true;
#else
	unsigned char counter = 0;
	SSPCON2bits.PEN = 1;
	while(SSPCON2bits.PEN)
	{
		if (255 == counter)
			return false;
		
		counter = counter + 1;
	}
	return true;
#endif
}

void I2cMasterIdle()
{
#if defined(_PIC18F14K50_H_)
	IdleI2C();
#else
	while ((SSPCON2 & 0x1F) | (SSPSTATbits.R_nW));
#endif
}

bool I2cMasterPut(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	unsigned char i = 0;
	unsigned char j = 0;
	I2cMasterIdle();
	for (; j < SEND_TRY; j++)
	{
		I2cMasterStart();

		/*
		 * because I will communicate always with one slave only I dont need send an
		 * address so I will use this required byte for a message type
		 * on the slave I will mask all the address byte out
		 */
		if (!I2cMasterWrite((command << 2) | (messageType << 1))) //lowest bite is read/write (write = 0)
		{
			I2cMasterStop();
			continue;
		}

		for (; i < count; i++)
		{
			if(!I2cMasterWrite(data[i]))
			{
				I2cMasterStop();
				goto continue2;
			}
		}
		break;
continue2:
;
	}
	if (i == SEND_TRY)
		LATC = 0b0000;
	I2cMasterStop();

	//FIXME: doesnt work, when slave is not connected WriteI2C finish correctly
	return (i == SEND_TRY);
}


bool I2cMasterGet(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count, unsigned char *retVal)
{
	unsigned timeout = 0xff;
	I2cMasterStart();
	
	while (!I2cMasterWrite((command << 2) | (messageType << 1) | 1))
	{
		if (0  == --timeout)
		{
			I2cMasterStop();
			return false;
		}
	}
	if(!I2cMasterRead(retVal))
	{
		I2cMasterStop();
		LATC = 0b0000;
		return false;
	}
	I2cMasterStop();
	LATC = 0b1111;
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
	return I2cMasterGet(I2C_MESSAGE_TYPE_COMMAND, command, 0, 0, retVal);
}

void CheckI2cAsSlave(void)
{
	unsigned char value;
	if (!SSP1IF) //MSSP interupt flag (SPI or I2C)
		return;

	SSP1IF = 0;
	
	value = SSPBUF;

	//FIXME: I should wait for processing last command or data
	if (!IS_DATA) //"address" byte in write mode
	{	
		g_stateFollowed = (0 == (value & 2)); //second lowest bite is I2C_MESSAGE_TYPE where 0 means data
		g_commandInstruction = (value >> 2);

		if (COMMAND_FLASH_GET_VERSION == g_commandInstruction)
		{
			g_bootloaderPolicy = 1;
			SSPBUF = 0; //I'm in program so i dont have a bootloader version
		}
		else if (COMMAND_GET_CURRENT_MODE == g_commandInstruction)
			SSPBUF = g_persistant.mode;

		//I want to be sure this insrtuction didn't come by a mistake
		//It is very important because after this isnstuction is chip set to
		//a bootloader state and the program is not accessible an more
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
}

bool SendMessageToOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	bool retVal;

	INnOUT_PORT = 0;

	I2cMasterInit();
	retVal = I2cMasterPut(messageType, command, data, count);
	I2cSlaveInit();

	INnOUT_PORT = 1;

	return retVal;
}

bool GetMessageFromOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count, unsigned char *value)
{
	bool retVal;

	INnOUT_PORT = 0;

	I2cMasterInit();
	retVal = I2cMasterGet(messageType, command, data, count, value);

	I2cSlaveInit();

	INnOUT_PORT = 1;

	return retVal;
}

void SendToOutputIfReady()
{
	if (g_toOutput.isReady)
	{
		if (TO_OUTPUT_MAX_TRAY == g_toOutput.try)
		{
			g_toOutput.isReady = false;
			//TODO: solve module on output is not connected an more
			return;
		}
		g_toOutput.try = g_toOutput.try + 1;
		if (g_toOutput.isState)
		{
			if (SendMessageToOutput(I2C_MESSAGE_TYPE_DATA, 0, &g_state, 1))
				g_toOutput.isReady = false;
			else //message was not send
				INVERT_OUTPUT_PORT = !INVERT_OUTPUT_PORT; //output device may be connected by an oposite way
		}
		else
		{
			//TODO: command send
		}

	}
}