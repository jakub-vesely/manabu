#include "common/common_16F1503.h"

void ProcessStateChangedModuleTypeSpecific()
{
	/*if (g_state == 0)
		g_state = 1;
	if (g_state == 255)
		g_state = 254;
	*/
}

void ModuleTypeSpecificInit()
{
	g_state = 0;
}

void ProcessModuleFunctionalit()
{
	/*if (g_state == 254)
		g_state = 1;
	else
		g_state = g_state + 1;
	Wait(1);
	g_stateChanged = true;*/
}
