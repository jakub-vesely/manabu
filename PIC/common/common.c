#include "common.h"

#ifndef LPCDEVKIT
#	include <system_common.h>
#	include <common/HEFlash.h>
#	include <common/i2c.h>
#endif

#define TO_OUTPUT_MAX_TRAY 10

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
	switch(g_commandInstruction)
	{
		case COMMAND_CHANGE_MODE:

			g_persistant.mode = g_commandValue;
#ifndef LPCDEVKIT
			HEFLASH_writeBlock(0, (void*)&g_persistant, sizeof(g_persistant));
#endif
			g_stateChanged = true;
		break;
		case COMMAND_FLASH_SET_BOOT_FLAG:
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

void ProcessStateChangedCommon()
{
	g_stateChanged = false;

#ifdef HAVE_OUTPUT
	g_toOutput.isState = true;
	g_toOutput.send_try = TO_OUTPUT_MAX_TRAY;
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
