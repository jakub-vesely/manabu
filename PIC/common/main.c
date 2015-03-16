#include <stdbool.h>
#include <common/common.h>
#include <common/common_16F1503.h>
#include <xc.h>
#include <moduleTypeSpecific.h>

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

void interrupt isr(void)
{
	ProcessI2cInterrupt();
}

void main(void)
{
	Common16F1503Init();
	ModuleTypeSpecificInit();

	while(1)
	{
		if (g_commandRecieved)
			ProcessCommand();

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