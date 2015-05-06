#include <stdbool.h>
#include <common/common.h>
#include <system_common.h>
#include <common/i2c.h>
#include <xc.h>
#include <moduleTypeSpecific.h>

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

void main(void)
{
	INTCONbits.GIE = 0;

	OSCCONbits.IRCF = 0b1111; //16MHz
	while (!OSCSTATbits.HFIOFS);
	
	g_persistant.mode = 1;
	g_persistant.bootLoaderCheck = RUN_PROGRAM_VALUE;
	Common16F1503Init();
	ModuleTypeSpecificInit();

	while(1)
	{

		CheckI2cAsSlave();
		if (g_commandRecieved)
			ProcessCommand();

		ProcessModuleFunctionalit();

		if (g_stateChanged)
		{
			ProcessStateChangedModuleTypeSpecific();
			ProcessStateChangedCommon();
		}
#ifdef HAVE_OUTPUT
		SendToOutputIfReady();
#endif //#ifdef HAVE_OUTPUT
	}
}