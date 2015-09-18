#include <xc.h>
#include <common/common.h>
#include <system_common.h>

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);


void SdaSclSwitch()
{
	LATC0 = 0;
	LATC1 = 1;
	Wait(10);


	LATC0 = 1;
	LATC1 = 0;
	Wait(10);
}
main()
{
	OSCCONbits.IRCF = 0b1111; //16MHz
	while (!OSCSTATbits.HFIOFS);

	ANSELA = 0x00;      //set analog pins to digital
    ANSELC = 0x00;

	SwitchControllerInit();

	TRISC0 = 0;
	LATC0 = 0;
	TRISC1 = 0;
	LATC1 = 0;

	INnOUT_PORT = 0;
	INVERT_OUTPUT_PORT = 1;

	while (1)
	{
        INnOUT_PORT = 1;
        SdaSclSwitch();
        
        INnOUT_PORT = 0;        
		INVERT_OUTPUT_PORT = 0;
		SdaSclSwitch();
        
        INVERT_OUTPUT_PORT = 1;
		SdaSclSwitch();
	}
}
