#include <system_common.h>
#include <common/common.h>
#include <CommonConstants.h>

void ProcessCommandModuleTypeSpecific()
{
}
void ProcessStateChangedModuleTypeSpecific()
{
	g_state = 255-g_state; //works as invertor
}

void ModuleTypeSpecificInit()
{
	g_state = 0;
}


void ProcessModuleFunctionality()
{
}

unsigned char GetModuleType()
{
	return TYPE_ARITHMETIC_LOGIC;
}
