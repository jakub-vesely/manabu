#include <system_common.h>
#include <common/common.h>
#include <CommonConstants.h>

#define MULTIPLIER 10
bool g_increase = true;
typedef enum
{
	MODE_SAW = 0,
    MODE_TRIANGLE = 1,	
    MODE_COUNT = 2
} MODE;

void ProcessCommandModuleTypeSpecific()
{
    
}
void ProcessStateChangedModuleTypeSpecific()
{
    switch (g_persistant.mode)
    {
        case MODE_SAW:
            g_outState += g_inState / MULTIPLIER;
            if (g_outState > STATE_MAX)
                g_outState = 0;
        break;
        case MODE_TRIANGLE:
            if (g_increase)
            {
                if (g_outState > STATE_MAX - g_inState / MULTIPLIER)
                {
                    g_outState = STATE_MAX;
                    g_increase = false;
                }
                else
                    g_outState += g_inState / MULTIPLIER;
                
                    
            }
            else
            {
                if (g_outState < g_inState / MULTIPLIER)
                {
                    g_outState = 0;
                    g_increase = true;
                }
                else
                    g_outState -= g_inState / MULTIPLIER;
                
                
            }
        break;
    }
    
    //}
	//g_outState = g_inState / 2;
    //g_outState = STATE_MAX - g_inState; //works as invertor
}

void ModuleTypeSpecificInit()
{
    g_outState = 0;
}


void ProcessModuleFunctionality()
{
}

unsigned char GetModuleType()
{
	return TYPE_GENERATOR;
}

unsigned char GetModeCount()
{
    return MODE_COUNT;
}
