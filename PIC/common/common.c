#include "common.h"

#ifndef LPCDEVKIT
#	include <CommonConstants.h>
#	include <system_common.h>
#	include <common/HEFlash.h>
#	include <common/i2c.h>
#endif

unsigned char g_invertOutput = 0;

void Wait(int delay)
{
	int i;
	int j;
	for (i = 0; i < 100 * delay; i++)
		for (j = 0; j < 10; j++);

}

void SwitchControllerInit()
{
#ifndef LPCDEVKIT
		INnOUT_TRIS = 0;
		INnOUT_PORT = 1;

#	if defined HAVE_OUTPUT
		INVERT_OUTPUT_TRIS = 0;
		INVERT_OUTPUT_PORT = 0;
#	endif
#endif
}

void ProcessCommandCommon()
{
	g_commandRecieved = false;
	//unsigned char deleteme;
	switch(g_commandInstruction)
	{
		/*case COMMAND_CHANGE_MODE:

			g_persistant.mode = g_commandValue;
#ifndef LPCDEVKIT
			HEFLASH_writeBlock(0, (void*)&g_persistant, sizeof(g_persistant));
#endif
			g_stateChanged = true;
		break;*/
		case MID_COMMAND_FLASH_SET_BOOT_FLAG:
			g_persistant.bootLoaderCheck = g_commandValue;
#ifndef LPCDEVKIT
			HEFLASH_writeBlock(0, (char*)&g_persistant, sizeof(g_persistant));
			Wait(1);
#endif
			//if (0 !=  g_persistant.bootLoaderCheck)
			{
#asm
				RESET
#endasm
			}
		break;
	}
}

void InvertOutput()
{
	g_invertOutput = (g_invertOutput) ? false : true;
	INVERT_OUTPUT_PORT = g_invertOutput;
}

bool SendMessageToOutput(unsigned char messageType, MessageId command, unsigned char const *data, unsigned char count)
{
#if defined(HAVE_OUTPUT)
	bool retVal = 0;
#	if defined(HAVE_INPUT)
		if (SSP1IF || SSPSTATbits.S) //message from input recieved or just comming
		{
			INPUT_MESSAGE_MISSED = false;
			return false;
		}
#	endif
	SSPEN = 0;
	INnOUT_PORT = 0; //sitch to output
#	if defined(HAVE_INPUT)
		INPUT_MESSAGE_MISSED = false; //it must be behind INnOUT_PORT = 0; because it cause iterupt
#	endif
	I2cMasterInit();
	I2cMasterIdle();
#	if defined(HAVE_INPUT)
		if (!INPUT_MESSAGE_MISSED)
			retVal = I2cMasterPut(messageType, command, data, count);
#	else
	retVal = I2cMasterPut(messageType, command, data, count);
#	endif
	I2cMasterIdle();
	SSPEN = 0;

	INnOUT_PORT = 1;
	I2cSlaveInit();
#	if defined(HAVE_INPUT)
		if (INPUT_MESSAGE_MISSED)
			g_inputMessageMissed = true;
#	endif
	return retVal;
#else
	return false;
#endif
}

bool SendCommand(MessageId command, unsigned char const * data, unsigned char count)
{
	unsigned tryCounter = TO_OUTPUT_MAX_TRAY;
	while (--tryCounter != 0) //I will repeat it couple of times because output module could be connected to an output and bss b sending a message
	{
		if (SendMessageToOutput(I2C_MESSAGE_TYPE_COMMAND, command, data, count))
			break;

		InvertOutput(); //may be output module is inverted
	}

	return (0 != tryCounter);
}


void CommonInit()
{
#ifndef LPCDEVKIT
	ANSELA = 0x00;      //set analog pins to digital
    ANSELC = 0x00;

	if (0 == HEFLASH_readByte(0, 0)) //is not the first time run after the program loading
		HEFLASH_readBlock((char *)&g_persistant, 0, sizeof(g_persistant));
#endif

#ifdef HAVE_OUTPUT
	SwitchControllerInit();
#endif

#ifndef LPCDEVKIT
	I2cSlaveInit();
#endif
	
#ifdef INTERUPTS_ENABLED
	INTCONbits.GIE = 1;
#endif
}
