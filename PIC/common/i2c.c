#include "i2c.h"

unsigned char g_stateFollowed = 0;

void I2cSlaveInit()
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
    SSPCON1 = 0b00010110;
//      WCOL = 0; //clear write collisions
//      SSPOV = 0; //clear receive overflow indicator
//      CKP = 1; //releases clock stretching
//      SSPM = 0b0110; //7-bit addressing slave mode

	/* Enable interrupts */
    SSP1IF  = 0; //Clear MSSP interrupt flag
    SSP1IE  = 1; //I2C interrupt enable
    PEIE    = 1; //Enable Peripheral interrupts
    GIE     = 1; //Enable global interrupts

	//GCEN =1; //General call address
	SSPEN = 1; //enable SSP and configures SDA & SCL pins
}

void I2cMasterInit(void)
{
	SSPEN = 0;
	//SDA andSCL port setting
	LATCbits.LATC0 = 0;
	LATCbits.LATC1 = 0;
	PORTCbits.RC0 = 0;
	PORTCbits.RC1 = 0;
	TRIS_SCL = 1; //as input
	TRIS_SDA = 1;
	ANSC0 = 0;
	ANSC1 = 0;

	//I2C master mode setting
	SDAHT = 1;			//Minimum of 300 ns hold time on SDAx after the falling edge of SCLx
	SMP = 1;           //Slew rate control disabled for standard speed mode (100 kHz and 1 MHz)
	CKE = 1;           //Transmit occurs on transition from active to Idle clock state

	SSPMSK = 0;
	SSPCON1 = 0; 
	SSPCON2 = 0;
	SSPCON3 = 0;
	SSPSTAT = 0;

	SSPADD = 39;		//100KHz
	SSPM3 = 1;         //Enable I2C Master mode
	SSPEN = 1;         //Enable SSP module - I2C Initialized
 }

void I2cMasterStart(void)
{
	/*SSPMSK = 0;
	SSPCON2 = 0;
	SSPCON3 = 0;
	SSPSTAT = 0;
	*/
	SSPCON2bits.SEN=1;
	while(SSPCON2bits.SEN);
}
/*
FIXME: legacy - returns false when success
*/
bool I2cMasterWrite(char byte)
{
	SSPBUF = byte;
	while(SSP1STATbits.R_nW);
	return SSPCON2bits.ACKSTAT;
}

char I2cMasterRead()
{
	SSPCON2bits.RCEN = 1;
	while(SSPCON2bits.RCEN);
	return SSPBUF;
}

void I2cMasterStop(void){
	SSPCON2bits.PEN = 1;           
	while(SSPCON2bits.PEN);
}

void I2cMasterPut(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	unsigned char i = 0;
	unsigned char try = 0;
	for (; try < 10; try++)
	{
		I2cMasterStart();

		/*
		 * because I will communicate always with one slave only I dont need send an
		 * address so I will use this required byte for a message type
		 * on the slave I will mask oall the address byte out
		 */
		if (I2cMasterWrite((command << 2) | (messageType << 1))) //lowest bite is read/write (write = 0)
		{
			I2cMasterStop(); //there maight be a complete I2C restart
			continue;
		}

		for (; i < count; i++)
		{
			if (I2cMasterWrite(data[i])) //FIXME: false when success
			{
				I2cMasterStop();
				goto next_turn;
			}
		}
		break;
next_turn:continue;
	}

	I2cMasterStop();
}

unsigned char I2cMasterGet(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	unsigned char value = 0;

	I2cMasterStart();
	//for an explanation take alook to PutI2C
	I2cMasterWrite((command << 2) | (messageType << 1) | 1); //lowest bite is read/write (write = 0)
	
	value = I2cMasterRead();
	I2cMasterStop();
	return value;
}



void ProcessI2cInterrupt()
{
	//FIXME: there is still one open isuue when I read a data Im not able to recieve broadcast data
	//untill first non broadcast data are recieved. It looks then, I get a zero address and read data (not address)

	if (!SSP1IF) //MSSP interupt flag (SPI or I2C)
		return;

	SSP1IF = 0;
	
	bool isData = SSPSTATbits.D_nA;
	bool isRead = SSPSTATbits.R_nW;
	unsigned char value;
	if (0 != SSPSTATbits.BF) //BF FIXME:actually I dont solve the case there is not data prepared
		value = SSPBUF;

	//FIXME: I should wait for processing last command or data
	if (!isData && !isRead) //"address" byte in write mode
	{	
		g_stateFollowed = (0 == (value & 2)); //second lowest bite is I2C_MESSAGE_TYPE where 0 means data
		g_commandInstruction = (value >> 2);
	}
	else if (!isRead) //isData
	{
		if (g_stateFollowed)
		{
			g_state = value;
			g_stateChanged = true;
		}
		else
		{
			g_commandValue = value;
			g_commandRecieved = true;
		}
	}
	else //isRead. I guess it will be always an address
	{
		g_stateFollowed = false; //read will be always called for commands only
		switch (value >> 2)
		{
			case COMMAND_GET_CURRENT_MODE:
				g_commandRecieved = false;
				//while(BF);      //wait while buffer is full
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


