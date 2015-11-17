#include <system_common.h>
#include <common/common.h>
#include <CommonConstants.h>

#define CALIBRATION 1.001

bool g_lastbuttomState = false;

typedef enum
{
	MODE_BUTTON = 0,
    MODE_MODE_SWITCHER = 1,	
    MODE_COUNT = 2
} MODE;

void ProcessCommandModuleTypeSpecific()
{
}
void ProcessStateChangedModuleTypeSpecific()
{
	
}

void ModuleTypeSpecificInit()
{
	g_inState = 1;
    TRISA1 = 1;
    LATA1 = 1;
}

void ProcessModuleFunctionality()
{
    if (!g_stateMessageEnabled) //try to send command 
        return;
    
	bool buttonState = RA1;
    
    switch (g_persistant.mode)
    {
        case MODE_BUTTON:
        {
            unsigned outState = (buttonState) ? g_inState : 0;
            if (g_outState != outState)
            {
                g_outState = outState;
                g_stateChanged = true;
            }
        }
        break;
        case MODE_MODE_SWITCHER:
            
            if (buttonState != g_lastbuttomState && buttonState == false) //click
            {
                g_toOutput.messageType = I2C_MESSAGE_TYPE_COMMAND;
                g_toOutput.commandId = MID_DECREASE_INCREASE_MODE;
                g_outState = INCREASE_MODE; //INCREASE
                g_stateChanged = true;
            }
            else if (g_stateMessageEnabled)
            {
                g_toOutput.messageType = I2C_MESSAGE_TYPE_DATA;
                g_toOutput.commandId = 0;
                g_outState = g_inState;
            }

            g_lastbuttomState = buttonState;
        break;
    }
}

unsigned char GetModuleType()
{
	return TYPE_BUTTON;
}

unsigned char GetModeCount()
{
    return MODE_COUNT;
}
