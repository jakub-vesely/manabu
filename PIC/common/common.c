#include "common.h"

#ifndef LPCDEVKIT
#	include <system_common.h>
#	include <common/HEFlash.h>
#endif
void Wait(int delay)
{
	int i;
	int j;
	for (i = 0; i < 100 * delay; i++)
		for (j = 0; j < 50; j++);

}

void SwitchControllerInit()
{
#ifndef LPCDEVKIT
#	if defined HAVE_OUTPUT
		INVERT_OUTPUT_TRIS = 0;
#	endif


		INnOUT_TRIS = 0;
		INnOUT_PORT = 1;

#	ifdef HAVE_OUTPUT
		INVERT_OUTPUT_PORT = 0;
#	endif
#endif
}

void ProcessCommand()
{
	g_commandRecieved = false;
	//unsigned char deleteme;
	if (COMMAND_CHANGE_MODE == g_commandInstruction )
	{
			g_persistant.mode = g_commandValue;
#ifndef LPCDEVKIT
			HEFLASH_writeBlock(0, (void*)&g_persistant, sizeof(g_persistant));
#endif
			g_stateChanged = true;
	}
	else if (COMMAND_FLASH_SET_BOOT_FLAG == g_commandInstruction)
	{
		g_persistant.bootLoaderCheck = g_commandValue;
#ifndef LPCDEVKIT
		HEFLASH_writeBlock(0, (char*)&g_persistant, sizeof(g_persistant));
#endif
		//if (0 !=  g_persistant.bootLoaderCheck)
		{
#asm
			RESET
#endasm
		}
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
