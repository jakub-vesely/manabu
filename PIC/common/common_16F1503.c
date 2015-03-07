#include "common_16F1503.h"

void Common16F1503Init()
{
	OSCCONbits.IRCF = 0b1111; //16MHz
	while (!OSCSTATbits.HFIOFS);

	ANSELA = 0x00;      //set analog pins to digital
    ANSELC = 0x00;
}

void Wait(int delay)
{
	int i;
	int j;
	for (i = 0; i < 100 * delay; i++)
		for (j = 0; j < 50; j++);

}