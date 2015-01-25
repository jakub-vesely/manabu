#include "i2c.h"

signed char g_commandPartFollowed = -1;

void I2cInit(unsigned char address)
{
    SSPEN = 0;

	ANSELC = 0x00;
	PORTC = 0;
	LATC = 0;

	TRIS_SCL = 1;
    TRIS_SDA = 1;


    SSPCON2 = 0b00000001; //SEN is set to enable clock stretching
//      SEN = 1; //Enable Clock Stretching

	SSPCON3 = 0x00;
    SSPADD = address << 1; //7-bit address is stored in the 7 MSB's of the SSP1ADD register**********
    SSPSTAT = 0x00;

    //SSP1CON1 bits with details
    SSPCON1 = 0b00110110;
//      WCOL = 0; //clear write collisions
//      SSPOV = 0; //clear receive overflow indicator
//      SSPEN = 1; //enable SSP and configures SDA & SCL pins
//      CKP = 1; //releases clock stretching
//      SSPM = 0b0110; //7-bit addressing slave mode

	/* Enable interrupts */
    SSP1IF  = 0; //Clear MSSP interrupt flag
    SSP1IE  = 1; //I2C interrupt enable
    PEIE    = 1; //Enable Peripheral interrupts
    GIE     = 1; //Enable global interrupts

	GCEN =1; //General call address

}

void ProcessI2cInterrupt()
{
	if (0 == SSPSTAT & 0b00000001) //it's mot I2C interrupt
		return;
	
	bool isData = SSPSTAT & 0b00100000;
	bool isRead = SSPSTAT & 0b00000100;
	unsigned char value;
	
	if (!isData && !isRead)
	{
		value = SSPBUF;
		if (0 != value) //data are spred as a broadcast command have an address
			g_commandPartFollowed = 0; //command is composed from two bytes comand Id + command value
	}
	else if (isData && !isRead)
	{
		value = SSPBUF;

		if (-1 != g_commandPartFollowed)
		{
			if (0 == g_commandPartFollowed)
			{
				g_commandInstruction = value;
				g_commandPartFollowed = 1;
			}
			else
			{
				g_commandValue = value;
				g_commandPartFollowed = -1;
				g_commandRecieved = true;
			}
		}
		else if (value != g_value)
		{
			g_value = value;
			g_valueChanged = 1;
		}
	}
	else //isRead. I guess it will be always an address
	{
		value = SSPBUF; //clear BF

		switch (g_commandInstruction)
		{
			case COMMAND_GET_CURRENT_MODE:
				g_commandRecieved = false;
				 SSPBUF = g_mode;
				break;
			default:
				 SSPBUF = 0;
		}
	}
	if (SEN)
		CKP = 1;
}


