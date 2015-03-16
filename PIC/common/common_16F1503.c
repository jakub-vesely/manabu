#include "common_16F1503.h"
#include "HEFlash.h"

#define TO_OUTPUT_MAX_TRAY 10

void Common16F1503Init()
{
	OSCCONbits.IRCF = 0b1111; //16MHz
	while (!OSCSTATbits.HFIOFS);

	ANSELA = 0x00;      //set analog pins to digital
    ANSELC = 0x00;

	g_mode = HEFLASH_readByte (1, 0);
	if (g_mode == 0xff) //default value
		g_mode = 1;
#ifdef HAVE_OUTPUT
	SwitchControllerInit();
#endif
	I2cSlaveInit();
}

void Wait(int delay)
{
	int i;
	int j;
	for (i = 0; i < 100 * delay; i++)
		for (j = 0; j < 50; j++);

}

void SwitchControllerInit()
{
	LATA4 = 0;
	LATA5 = 0;
	TRISAbits.TRISA4 = 0;
	TRISAbits.TRISA5 = 0;
	OUTPUT_SWITCH = 0;
	INPUT_SWITCH = 1;
}

bool SendMessageToOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	bool retVal;
	INPUT_SWITCH = 0;
	OUTPUT_SWITCH = 1;
	I2cMasterInit();
	retVal = I2cMasterPut(messageType, command, data, count);
	I2cSlaveInit();
	OUTPUT_SWITCH = 0;
	INPUT_SWITCH = 1;

	return retVal;
}

unsigned char GetMessageFromOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	unsigned char value;
	INPUT_SWITCH = 0;
	OUTPUT_SWITCH = 1;
	I2cMasterInit();
	value = I2cMasterGet(messageType, command, data, count);
	I2cSlaveInit();
	OUTPUT_SWITCH = 0;
	INPUT_SWITCH = 1;

	return value;
}

void ProcessCommand()
{
	//unsigned char deleteme;
	switch(g_commandInstruction)
	{
		case COMMAND_CHANGE_MODE:
			g_commandRecieved = false;
			g_mode = g_commandValue;
			HEFLASH_writeBlock(1, (void*)&g_mode, sizeof(g_mode)); //I dont understand why but radd = 0 doesn't work for me
			g_stateChanged = true;
			break;
	}
}

void ProcessStateChangedCommon()
{
	g_stateChanged = false;

#ifdef HAVE_OUTPUT
	g_toOutput.isState = true;
	g_toOutput.try = 0;
	g_toOutput.isReady = true;		
#endif
}

void SendToOutputIfReady()
{
#ifdef HAVE_OUTPUT
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
		}
		else
		{
			//TODO: command send
		}

	}
#endif //#ifdef HAVE_OUTPUT
}