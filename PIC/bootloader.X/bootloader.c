#include<xc.h>
#include <system_common.h>
#include <common/i2c.h>
#include <common/common.h>

#include <common/Flash.h>
#include <stdbool.h>
#include <CommonConstants.h>

#if defined (INTERFACE)
// CONFIG1
#	pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#	pragma config WDTE = SWDTEN   // Watchdog Timer Enable (WDT controlled by software)
#	pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#	pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#	pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#	pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#	pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#	pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
#	pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#	pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#	pragma config CPUDIV = NOCLKDIV// CPU System Clock Selection Bit (NO CPU system divide)
#	pragma config USBLSCLK = 48MHz // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
#	pragma config PLLMULT = 3x     // PLL Multipler Selection Bit (3x Output Frequency Selected)
#	pragma config PLLEN = ENABLED  // PLL Enable Bit (3x or 4x PLL Enabled)
#	pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#	pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#	pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#	pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#	define VERSION 2
#	define MAIN_PROOGRAM_MAX 0x1FFF
#else
// CONFIG1
#	pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#	pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#	pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#	pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#	pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#	pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#	pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#	pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#	pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#	pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#	pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#	pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#	define VERSION 1
#	define MAIN_PROOGRAM_MAX 0x7FF
#endif

#define MAIN_PROOGRAM_START 0x100
#define RUN_PROGRAM_FLAG_POSITION HEFLASH_START

unsigned g_flashAddr;
unsigned char g_command = 0;
unsigned g_word;
unsigned checkSum = 0;
struct g_bitFiled
{
	bool dataReady:1;
	bool lo:1;
} g_bitFiled;

void ReadI2C()
{
	if (!SSP1IF) //MSSP interupt flag (SPI or I2C)
		return;

	SSP1IF = 0;

	unsigned char value = SSPBUF;

	if (!IS_DATA)
	{
		g_command = value >> 2;

		if (MID_COMMAND_FLASH_GET_VERSION == g_command)
		{
			SSPBUF = VERSION;
		}
		else if (MID_COMMAND_FLASH_CHECKSUM == g_command)
		{
			SSPBUF = checkSum;
			checkSum = 0; //read for next programming
		}
		CKP = 1;
	}
	else
	{
		CKP = 1;

		checkSum += value;

		if (g_bitFiled.lo)
		{
			g_bitFiled.lo = false;
			g_word = value;
		}
		else
		{
			g_bitFiled.lo = true;
			g_word |= (value << 8);

			if (MID_COMMAND_FLASH_ADDRESS == g_command)
			{
				g_flashAddr = g_word;

				//I dont want to use flash protection in config flags because
				//I would have to have booltloader sise 0x200 so I will protect
				//it this way. I probabl shoud increase checksum but I will have
				//to consume a more memory space. It will not happend by
				//a correct way and checksum will not maptch anyway
				if (g_flashAddr < MAIN_PROOGRAM_START || g_flashAddr > MAIN_PROOGRAM_MAX)
					g_flashAddr = MAIN_PROOGRAM_START;

				FLASH_ERASE(g_flashAddr);
			}
			else if (MID_COMMAND_FLASH_LATCH_WORD == g_command || MID_COMMAND_FLASH_WRITE_WORD == g_command)
				g_bitFiled.dataReady = true;
		}
	}
}

void interrupt serrvice_isr()
{
	#asm
		GOTO (MAIN_PROOGRAM_START + 4);
	#endasm
}

int main()
{
	//NOTE: setting diferent frequenc cause slower programming, I don't understand why
	//OSCCONbits.IRCF = 0b1111;
	//while (!OSCSTATbits.HFIOFS);

	INTCONbits.GIE = 0;
	
	TRISA5 = 1;
	FLASH_READ_BYTE(RUN_PROGRAM_FLAG_POSITION)
    
    //NOTE: when you will activate RA5 there must be RA4 for LIPOL version > 0.1
	if (RUN_PROGRAM_VALUE == PMDAT )//&& PORTAbits.RA5)   
	{
#asm
		GOTO MAIN_PROOGRAM_START;
#endasm
	}

	g_bitFiled.dataReady = false;
	g_bitFiled.lo = true;
	ANSELC = 0; //no analog pins

	//swich is set to input
	INnOUT_TRIS = 0;
	INnOUT_PORT = 1;

	I2C_COMMON_INIT
	I2C_SLAVE_SPECIFIC_INIT

	while (1)
	{
		if(g_command == MID_COMMAND_FLASH_END)
		{
#asm
			GOTO MAIN_PROOGRAM_START;
#endasm
		}

		ReadI2C();

		if (g_bitFiled.dataReady)
		{
			g_bitFiled.dataReady = false;
			FLASH_WRITE(g_flashAddr, g_word, MID_COMMAND_FLASH_LATCH_WORD == g_command);
			if (PMCON1bits.WRERR) //0 write success, 1 write error
				++checkSum;
			g_flashAddr++;
		}
	}
}