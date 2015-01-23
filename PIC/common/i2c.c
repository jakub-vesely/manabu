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

void ProcessI2cInterrupt(unsigned char status)
{
	unsigned char value;
	status = (status & 0b00101101);
	switch (status)
	{
		//I2C write, last byte was an address
		//SSPSTAT bits: D/A=0, S=1, R/W=0, BF=1
		case 0b00001001:
			value = SSPBUF;         //read buffer, clear BF
			if (0 != value) //data are spred as a broadcast command have an address
				g_commandPartFollowed = 0; //command is composed from two bytes comand Id + command value

			if (SEN)
				CKP = 1;           //release the SCL line
		break;

		//I2C write, last byte was data
		//SSPSTAT bits D/A=1, S=1, R/W=0, BF=1
		case 0b00101001:
			value = SSPBUF; //read buffer, clear BF
			if (SEN)
				CKP = 1;

			if (-1 != g_commandPartFollowed)
			{
				if (0 == g_commandPartFollowed)
				{
					g_commandInstruction = value;
					g_commandPartFollowed = 1;
				}

				g_commandValue = value;
				g_commandPartFollowed = -1;
				g_commandRecieved = true;
			}
			else if (value != g_value)
			{
				g_value = value;
				g_valueChanged = 1;
			}

			/*TODO: i2c body*/
		break;

		//may be later
		/*
		//I2C read, last byte was address
		//SSPSTAT bits: D/A = 0, S=1, R/W=1, BF=0
		case 0b00001101:    //possibly BF==1
		break;
		*/

	}
}


