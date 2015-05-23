#include <stdbool.h>
#include <common/common.h>
#include <system_common.h>
#include <common/i2c.h>
#include <xc.h>
#include <moduleTypeSpecific.h>

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#define TO_OUTPUT_MAX_TRAY 10
#define RECEIVE_TRY_COUNT  5000

unsigned char g_invertOutput = 0;

void EmintResponseToPred()
{
	unsigned char counter = 100;
	SDA_TRIS = false;
	SDA_PORT = false;
	while (0 != --counter)
	{}
	SDA_TRIS = true; //back to imput 1 will be set by a pull-up resistor
}

bool SendMessageToOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	SSPEN = 0;
	bool retVal = 0;

	INnOUT_PORT = 0;
	//it must be behind INnOUT_PORT = 0; because it cause iterupt
	INPUT_MESSAGE_MISSED = false;

	//I2cMasterInit();
	//if (!INPUT_MESSAGE_MISSED)
	//	retVal = I2cMasterPut(messageType, command, data, count);
	SSPEN = 0;
	
	INnOUT_PORT = 1; //It couldn't cause an interrupt because it cause on fall edge

	if (INPUT_MESSAGE_MISSED)
		EmintResponseToPred();

	SSP1STAT &= 0x3F;                // power on state
	SSP1CON1 = 0x00;                 // power on state
	SSP1CON2 = 0x00;

	SSPCON2 = 0b00000001; /*SEN is set to enable clock stretching*/
    SSPCON3 = 0x00;
    SSPMSK = 0x00; /*all address bits will be ignored*/
    SSPADD = 0x00; /*no address is set, all the time is conneceted only one slave*/
    SSPCON1 = 0b00010110; /*clock stretching + 7-bit addressing*/
  	SSPEN = 1;
	
	return retVal;
}

bool GetMessageFromOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count, unsigned char *value)
{
	bool retVal;

	INnOUT_PORT = 0;
	INPUT_MESSAGE_MISSED = false;
	I2cMasterInit();
	retVal = I2cMasterGet(messageType, command, value);

	I2cSlaveInit();

	//TODO: if EmintResponseToPred();
	INnOUT_PORT = 1;
	return retVal;
}

void SendToOutputIfReady()
{
	if (!g_toOutput.isReady)
		return;

	/*PORTCbits.RC5 = 1;
	Wait(1);
	PORTCbits.RC5 = 0;
	*/
	if (TO_OUTPUT_MAX_TRAY == g_toOutput.try)
	{
		g_toOutput.isReady = false;
		//TODO: solve module on output is not connected any more
		return;
	}
	g_toOutput.try = g_toOutput.try + 1;
	if (g_toOutput.isState)
	{
		if (SendMessageToOutput(I2C_MESSAGE_TYPE_DATA, 0, &g_state, 1))
		{
			g_toOutput.isReady = false;
		}
		/*else if (!INPUT_MESSAGE_MISSED)//message was not send
		{
			if (g_invertOutput)
			{
				g_invertOutput = 0;
				INVERT_OUTPUT_PORT = 0;
				//PORTCbits.RC5 = 1;
				//Wait(10);
			}
			else
			{
				g_invertOutput = 1;
				INVERT_OUTPUT_PORT = 1;
				//PORTCbits.RC5 = 0;
				//Wait(10);
			}
		}*/
	}
	else
	{
		//TODO: command send
	}
}

void main(void)
{
	INTCONbits.GIE = 0;

	OSCCONbits.IRCF = 0b1111; //16MHz
	while (!OSCSTATbits.HFIOFS);

	TRISA = 0x0; //mainly RA0 and RA1 should be as configured as outputs because the could cause external interupt which I use for Input bus checking
	TRISAbits.TRISA2 = 1; //input for iterupt

	INPUT_MESSAGE_MISSED = false; //interupt flag cleared
	OPTION_REGbits.INTEDG = 0; //external interupt to falling edge

	g_persistant.mode = 1;
	g_persistant.bootLoaderCheck = RUN_PROGRAM_VALUE;
	CommonInit();
	ModuleTypeSpecificInit();

	while(1)
	{
#if defined(HAVE_INPUT)
		CheckI2cAsSlave();

		INTF = 0; //i2c message has benn processed, clear input interrupt flag

		if (g_commandRecieved)
			ProcessCommand();
#endif

		ProcessModuleFunctionality();

		if (g_stateChanged)
		{
			ProcessStateChangedModuleTypeSpecific();
			ProcessStateChangedCommon();
		}
#if defined (HAVE_OUTPUT)
		SendToOutputIfReady();
#endif
	}
}