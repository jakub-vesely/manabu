#include <system_common.h>
#include <common/common.h>
#include <CommonConstants.h>

#define CALIBRATION 1.001

void ProcessCommandModuleTypeSpecific()
{
}
void ProcessStateChangedModuleTypeSpecific()
{
	
}

void ModuleTypeSpecificInit()
{
	g_inState = 1;
	TRISC5 = 0; //for diode

	//for potentiometer
	TRISCbits.TRISC3 = 1;
	ANSELCbits.ANSC3 = 1;

	ADCON0bits.CHS = 0b00111;

	ADCON1 = 0b11100000;
	ADCON2 = 0;

	ADCON0bits.ADON = 1;
}

unsigned int ADC_Read10bit(void)
{
    unsigned int result;

	ADCON0bits.GO = 1;              // Start AD conversion
    while(ADCON0bits.GO_nDONE)
	{} // Wait for conversion

    result = ADRESH;
    result <<=8;
    result |= ADRESL;

    //It took too mach program space (463 bytes))
    //return (unsigned)((float)(STATE_MAX - result) * CALIBRATION);
    
    //this code do the same but take only 32bytes
    //I need this code to cancel a noise near to max output value 
    if (result < STATE_MAX/ 2)
        return (STATE_MAX - result) + 1;
    else
        return (STATE_MAX - result);
}


void ProcessModuleFunctionality()
{
	unsigned potValue = ADC_Read10bit();
	if (potValue > STATE_MAX)
		potValue = STATE_MAX;
	unsigned outState = (unsigned)((unsigned long)g_inState * (unsigned long)potValue / (unsigned long)STATE_MAX);
	if (g_outState != outState)
	{
		g_outState = outState;
		g_stateChanged = true;
	}
}

unsigned char GetModuleType()
{
	return TYPE_POTENTIOMETER;
}
