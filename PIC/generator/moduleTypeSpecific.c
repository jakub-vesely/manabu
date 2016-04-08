#include <system_common.h>
#include <common/common.h>
#include <CommonConstants.h>

void ProcessCommandModuleTypeSpecific()
{
}
void ProcessStateChangedModuleTypeSpecific()
{
	
}

void ModuleTypeSpecificInit()
{
	g_inState = 1;
}




void ProcessModuleFunctionality()
{
	g_outState = g_inState;
}

unsigned char GetModuleType()
{
	return TYPE_POTENTIOMETER;
}
