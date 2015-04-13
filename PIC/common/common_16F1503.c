#include "common_16F1503.h"
#include "HEFlash.h"
#include "common.h"
#include "i2c.h"


void Common16F1503Init()
{
	OSCCONbits.IRCF = 0b1111; //16MHz
	while (!OSCSTATbits.HFIOFS);

	ANSELA = 0x00;      //set analog pins to digital
    ANSELC = 0x00;

	HEFLASH_readBlock((char *)&g_persistant, 0, sizeof(g_persistant));
	
	
	//if (g_persistant.g_mode == 0xff) //default value
	//	g_mode = 1;
	
#ifdef HAVE_OUTPUT
	SwitchControllerInit();
#endif
	I2cSlaveInit();

#ifdef INTERUPTS_ENABLED
	INTCONbits.GIE = 1;
#endif
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
#if defined HAVE_OUTPUT
	INVERT_OUTPUT_TRIS = 0;
#endif
	INnOUT_TRIS = 0;

#ifdef FOUR_PIN_INTERFACE
	OUTPUT_SWITCH = 0;
	INPUT_SWITCH = 1;
#else
	INnOUT_PORT = 1;
#	ifdef HAVE_OUTPUT
		INVERT_OUTPUT_PORT = 0;
#	endif
#endif

}

void ProcessCommand()
{
	//unsigned char deleteme;
	switch(g_commandInstruction)
	{
		case COMMAND_CHANGE_MODE:
			g_commandRecieved = false;
			g_persistant.mode = g_commandValue;
			HEFLASH_writeBlock(0, (void*)&g_persistant, sizeof(g_persistant));
			g_stateChanged = true;
			break;
		case COMMAND_FLASH_LOAD_CHECK:
			HEFLASH_writeBlock(0, (char*)&g_persistant, sizeof(g_persistant));
			if (0 !=  g_commandValue)
			{
#asm
	RESET
#endasm
			}
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