#include <system_common.h>
#include <common/common.h>
#include <CommonConstants.h>

void ProcessCommandModuleTypeSpecific()
{
}
void ProcessStateChangedModuleTypeSpecific()
{
	//g_outState = g_inState / 2;
    g_outState = STATE_MAX - g_inState; //works as invertor
}

void ModuleTypeSpecificInit()
{
}


void ProcessModuleFunctionality()
{
}

unsigned char GetModuleType()
{
	return TYPE_ARITHMETIC_LOGIC;
}

unsigned char GetModeCount()
{
    return 1;
}