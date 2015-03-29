#include <xc.h>
#include <common/common_16F1503.h>

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);


void SdaSclSwitch()
{
	PORTCbits.RC0 = 0;
	PORTCbits.RC1 = 1;
	Wait(10);


	PORTCbits.RC0 = 1;
	PORTCbits.RC1 = 0;
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
	PORTCbits.RC0 = 0;
	TRISC1 = 0;
	LATC1 = 0;
	PORTCbits.RC1 = 1;

	INnOUT = 0;
	INVERT_OUTPUT = 1;

	while (1)
	{
		INVERT_OUTPUT = 0;
		INnOUT = 1;
		SdaSclSwitch();

		INnOUT = 0;
		SdaSclSwitch();


		INVERT_OUTPUT = 1;
		INnOUT = 1;
		SdaSclSwitch();

		INnOUT = 0;
		SdaSclSwitch();
	}
}
