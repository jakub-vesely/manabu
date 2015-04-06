#include <xc.h>
#include <pic16f1503.h>

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

void main(void)
{
	//LATC5 = 0;
	TRISC5 = 0;
	PORTCbits.RC5 = 1;
	while(1)
	{
		//int i = 0;
		
		//for (;i < 2000; i++);

		//PORTCbits.RC5 = 0;
		//for (i = 0;i < 2000; i++);

	}
}