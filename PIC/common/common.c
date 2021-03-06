#include "common.h"
#include <CommonConstants.h>
#include <system_common.h>
#include <common/HEFlash.h>
#include <common/i2c.h>

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

	INnOUT_TRIS = 0;
	INnOUT_PORT = 1;

	INVERT_OUTPUT_TRIS = 0;
	INVERT_OUTPUT_PORT = 0;

}

void SetMode(unsigned char mode)
{
	g_persistant.mode = mode;

	HEFLASH_writeBlock(0, (char*)&g_persistant, sizeof(g_persistant));
	Wait(1);

	g_stateChanged = true;
}

void GoToBootloader(unsigned char value)
{
	g_persistant.bootLoaderCheck = value;
	HEFLASH_writeBlock(0, (char*)&g_persistant, sizeof(g_persistant));
	Wait(1);
	//if (0 !=  g_persistant.bootLoaderCheck)
	{
#asm
		RESET
#endasm
	}
}
void ProcessCommandCommon()
{
	g_commandRecieved = false;
	//unsigned char deleteme;
	switch(g_commandInstruction)
	{
		case MID_SET_MODE:
			SetMode(g_commandValue);
		break;
		case MID_COMMAND_FLASH_SET_BOOT_FLAG:
			GoToBootloader(g_commandValue);
		break;
        case MID_DECREASE_INCREASE_MODE:
            if (g_commandValue == INCREASE_MODE)
            {
                if (g_persistant.mode + 1 == GetModeCount())
                    SetMode(0);
                else
                    SetMode(g_persistant.mode + 1);
            }
            else
            {
                if (g_persistant.mode - 1 == 0)
                    SetMode(GetModeCount() - 1);
                else
                    SetMode(g_persistant.mode - 1);
            }
	}

	g_stateMessageEnabled = true; //command processed, we dont have to block the state sending
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
	INVERT_OUTPUT_TRIS = false; //I switch it to write state only for message writing and after that I will switch it back to read state. it looks is more efficient to power saving (current

	unsigned tryCounter = TO_OUTPUT_MAX_TRAY;
	while (--tryCounter != 0) //I will repeat it couple of times because output module could be connected to an output and bss b sending a message
	{
		if (SendMessageToOutput(I2C_MESSAGE_TYPE_COMMAND, command, data, count))
			break;

		InvertOutput(); //may be output module is inverted
	}

	INVERT_OUTPUT_TRIS = true; //I switch it to write state only for message writing and after that I will switch it back to read state. it looks is more efficient to power saving (current)


	return (0 != tryCounter);
}

unsigned char GetFromI2C(MessageId command, unsigned char* data, unsigned char count)
{
	unsigned tryCounter = TO_OUTPUT_MAX_TRAY;
	INVERT_OUTPUT_TRIS = false; //I switch it to write state only for message writing and after that I will switch it back to read state. it looks is more efficient to power saving (current)

	while (--tryCounter != 0) //I will repeat it couple of times because output module could be connected to an output and bss b sending a message
	{
		if (GetCommandFromI2C(command, data, count))
		{
			INVERT_OUTPUT_TRIS = true;
			return count;
		}

		InvertOutput(); //may be output module is inverted
	}

	INVERT_OUTPUT_TRIS = true;
	return 0;
}


void CommonInit()
{
	ANSELA = 0x00;      //set analog pins to digital
    ANSELC = 0x00;

	if (0 == HEFLASH_readByte(0, 0)) //is not the first time run after the program loading
		HEFLASH_readBlock((char *)&g_persistant, 0, sizeof(g_persistant));

#if defined(HAVE_INPUT) && defined(HAVE_OUTPUT)
	SwitchControllerInit();
#endif

#if defined (HAVE_INPUT)
	I2cSlaveInit();
#endif
    
#if defined (HAVE_OUTPUT)
    g_toOutput.commandId = 0;
    g_toOutput.isReady = false;
    g_toOutput.messageType = I2C_MESSAGE_TYPE_DATA;
    g_toOutput.sendTry = 0;
#endif

#ifdef INTERUPTS_ENABLED
	INTCONbits.GIE = 1;
#endif
}
