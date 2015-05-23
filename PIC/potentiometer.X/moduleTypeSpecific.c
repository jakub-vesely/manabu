#include <system_common.h>
#include <common/common.h>

//#define HISTERESIS

void ProcessStateChangedModuleTypeSpecific()
{
	
}

void ModuleTypeSpecificInit()
{
	g_state = 1;
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

    return result;
}
#ifdef HISTERESIS
#	define IS_DIFFERENT(potValue) potValue != g_state && potValue != g_state + 1 && potValue != g_state - 1
#else
#	define IS_DIFFERENT(potValue) potValue != g_state
#endif

void ProcessModuleFunctionality()
{
	unsigned char potValue = (unsigned char)(ADC_Read10bit() / 4);

	if (IS_DIFFERENT(potValue))
	{
		g_state = potValue;
		g_stateChanged = true;
	}
}
