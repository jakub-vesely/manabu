#include "i2c.h"

unsigned char g_stateFollowed = 0;

#define IS_DATA SSPSTATbits.D_nA
#define IS_READ  SSPSTATbits.R_nW

#define  I2C_COMMON_INIT\
	SSPEN = 0;\
	ANSC0 = 0;\
	ANSC1 = 0;\
	PORTCbits.RC0 = 0;\
	PORTCbits.RC1 = 0;\
	LATCbits.LATC0 = 0;\
	LATCbits.LATC1 = 0;\
	TRISCbits.TRISC0 = 1;\
	TRISCbits.TRISC1 = 1;

void I2cSlaveInit()
{
	I2C_COMMON_INIT

	SSPCON2 = 0b00000001; //SEN is set to enable clock stretching
//      SEN = 1; //Enable Clock Stretching
	SSPCON3 = 0x00;
	SSPMSK = 0; //all address bits will be ignored
	SSPADD = 0;//address << 1; //7-bit address is stored in the 7 MSB's of the SSP1ADD register**********
    SSPSTAT = 0x00;
    SSPCON1 = 0b00010110;
//      WCOL = 0; //clear write collisions
//      SSPOV = 0; //clear receive overflow indicator
//      CKP = 1; //releases clock stretching
//      SSPM = 0b0110; //7-bit addressing slave mode

	SSPEN = 1; //enable SSP and configures SDA & SCL pins
}
#ifdef HAVE_OUTPUT

	void I2cMasterInit(void)
	{
		I2C_COMMON_INIT

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
		return 0 == SSPCON2bits.ACKSTAT;
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
	/*
	 tady to zasekava spis by bylo dobry vratit se do zakadniho cyklu a pri dalsim
	 * pruchodu to zkusit znovu. Neni mozne ukladat zpráv do fronty, protože budou
	 * příliš dlouhé a RAM příliš maplá. ono b to ani nemělo být uné stav se bude
	 * přenášet vždy jen jeden a zpráva od remote zařízení taky. Bude ted jeden byte
	 * pro každého následovníka kam se boude ukládat počet pokusů o odeslání.
	 * pokud se mu to nepovede prohlásí zprávu za neodeslanou.
	 * jeden bit by mohla být informace o tom, zda se snaží odeslat data nebo stav.
	 * to b mělo tu výhodu,že b mohl přijmou příkazový řetězec i kdž se aktuálně
	 * snaží poslat stav tak b tím blo zajištěno, že vždck pošle aktuální stav když
	 * se v průběhu odesílacích pokusů stav změní.
	 *
	 */

	bool I2cMasterPut(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
	{
		unsigned char i = 0;

		I2cMasterStart();

		/*
		 * because I will communicate always with one slave only I dont need send an
		 * address so I will use this required byte for a message type
		 * on the slave I will mask oall the address byte out
		 */
		if (!I2cMasterWrite((command << 2) | (messageType << 1)))
		{
			I2cMasterStop(); //there maight be a complete I2C restart
			return false;
		}

		for (; i < count; i++)
		{
			if (!I2cMasterWrite(data[i])) //FIXME: false when success
			{
				I2cMasterStop();
				return false;
			}
		}

		I2cMasterStop();
		return true;
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
#endif //#ifdef HAVE_OUTPUT

void CheckI2C()
{
	if (!SSP1IF) //MSSP interupt flag (SPI or I2C)
		return;

	SSP1IF = 0;
	
	unsigned char value = SSPBUF;

	//FIXME: I should wait for processing last command or data
	if (!IS_DATA && !IS_READ) //"address" byte in write mode
	{	
		g_stateFollowed = (0 == (value & 2)); //second lowest bite is I2C_MESSAGE_TYPE where 0 means data
		g_commandInstruction = (value >> 2);
	}
	else if (!IS_READ) //isData
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