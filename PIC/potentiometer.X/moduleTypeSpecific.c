#include <system_common.h>
#include <common/common.h>

unsigned char g_potValue = 0;
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

	ADCON0bits.CHS = 0b00111;
    ADCON0bits.GO = 1;              // Start AD conversion
    while(ADCON0bits.GO_nDONE);     // Wait for conversion

    result = ADRESH;
    result <<=8;
    result |= ADRESL;

    return result;
}

void ProcessModuleFunctionalit()
{
	unsigned int potValue10 = ADC_Read10bit();

	if (potValue10 / 4 != g_potValue)
	{
		g_potValue = potValue10 / 4;
		g_state = g_potValue;
		g_stateChanged = true;
	}
}
