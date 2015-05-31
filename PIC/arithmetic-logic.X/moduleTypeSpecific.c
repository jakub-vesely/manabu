#include <system_common.h>
#include <common/common.h>
#include <CommonConstants.h>

void ProcessStateChangedModuleTypeSpecific()
{
	
}

void ModuleTypeSpecificInit()
{
	g_state = 0;
}


void ProcessModuleFunctionality()
{
	if (g_stateChanged)
	{
		g_state = 255-g_state; //works as invertor
	}
}

unsigned char GetModuleType()
{
	return TYPE_ARITHMETIC_LOGIC;
}
