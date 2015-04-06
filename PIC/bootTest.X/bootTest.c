#include <xc.h>
#include <pic16f1503.h>

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

void main(void)
{
	//LATC5 = 0;
	TRISC5 = 0;
	while(1)
	{
		PORTCbits.RC5 = 1;
		int i = 2000;
		
		while( i-- > 0);

		PORTCbits.RC5 = 0;
		i = 2000;
		while( i-- > 0);

	}
}