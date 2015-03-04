#include "i2c.h"

unsigned char g_valueFollowed = 0;

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

	SSPMSK = 0; //all address bits will be ignored
	SSPADD = 0;//address << 1; //7-bit address is stored in the 7 MSB's of the SSP1ADD register**********
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

	//GCEN =1; //General call address

}

void ProcessI2cInterrupt()
{
	//FIXME: there is still one open isuue when I read a data Im not able to recieve broadcast data
	//untill first non broadcast data are recieved. It looks then, I get a zero address and read data (not address)

	if (!SSP1IF) //MSSP interupt flag (SPI or I2C)
		return;

	SSP1IF = 0;

	bool isData = SSPSTAT & 0b00100000;
	bool isRead = SSPSTAT & 0b00000100;
	unsigned char value;
	if (0 != SSPSTAT & 0b00000001) //BF FIXME:actually I dont solve the case there is not data prepared
		value = SSPBUF;
	if (!isData && !isRead) //"address" byte in write mode
	{
		g_valueFollowed = (0 == (value & 2)); //second lowest bite is I2C_MESSAGE_TYPE where 0 means data
		g_commandInstruction = (value >> 2);
	}
	else if (!isRead) //isData
	{
		if (g_valueFollowed)
		{
			//FIXME: it should be renamed to state
			g_value = value;
			g_valueChanged = true;
		}
		else
		{
			g_commandValue = value;
			g_commandRecieved = true;
		}
	}
	else //isRead. I guess it will be always an address
	{
		g_valueFollowed = false; //read will be always called for commands only
		switch (value << 2) 
		{
			case COMMAND_GET_CURRENT_MODE:
				g_commandRecieved = false;
				while(BF);      //wait while buffer is full
				SSPBUF = g_mode;
				break;
			default:
				SSPBUF = 0; //FIXME: it should not happend I don't it cause probably something wrong
				break;
		}
	}
	
	if (SEN)
		CKP = 1;
}


