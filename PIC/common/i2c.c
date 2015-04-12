#include "i2c.h"

unsigned char g_stateFollowed = 0;

#define TO_OUTPUT_MAX_TRAY 10
#define IS_DATA SSPSTATbits.D_nA
#define IS_READ  SSPSTATbits.R_nW

void I2cSlaveInit()
{
	I2C_COMMON_INIT
	I2C_SLAVE_SPECIFIC_INIT
}

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

bool I2cMasterStart(void)
{
	/*SSPMSK = 0;
	SSPCON2 = 0;
	SSPCON3 = 0;
	SSPSTAT = 0;
	*/
	unsigned char counter = 0;
	SSPCON2bits.SEN=1;
	while(SSPCON2bits.SEN)
	{
		if (255 == counter)
			return false;

		counter = counter + 1;
	}
	return true;
}
/*
FIXME: legacy - returns false when success
*/
bool I2cMasterWrite(char byte)
{
	unsigned char counter = 0;
	SSPBUF = byte;
	while(SSP1STATbits.R_nW)
	{
		if (255 == counter)
			return false;
		
		counter = counter + 1;
	}
	return 0 == SSPCON2bits.ACKSTAT;
}

char I2cMasterRead()
{
	unsigned char counter = 0;

	SSPCON2bits.RCEN = 1;
	while(SSPCON2bits.RCEN) //FIXME: may be ther should be !BF
	{
		if (255 == counter)
			return false; //FIXME FIXME FIXME: it should be returned an error value

		counter = counter + 1;
	}
	return SSPBUF;
}

bool I2cMasterStop(void){
	unsigned char counter = 0;
	SSPCON2bits.PEN = 1;
	while(SSPCON2bits.PEN)
	{
		if (255 == counter)
			return false;
		
		counter = counter + 1;
	}
	return true;
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

	if (!I2cMasterStart())
		return false;

	/*
	 * because I will communicate always with one slave only I dont need send an
	 * address so I will use this required byte for a message type
	 * on the slave I will mask oall the address byte out
	 */
	if (!I2cMasterWrite((command << 2) | (messageType << 1)))
	{
		I2cMasterStop(); //there may be a complete I2C restart
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

	return I2cMasterStop();
}

unsigned char I2cMasterGet(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	unsigned char value = 0;

	if (!I2cMasterStart())
		return 0; //FIXME FIXME FIXME: it should not be returned valid value

	//for an explanation take alook to PutI2C
	I2cMasterWrite((command << 2) | (messageType << 1) | 1); //lowest bite is read/write (write = 0)

	value = I2cMasterRead();
	I2cMasterStop();
	return value;
}

void CheckI2cAsSlave()
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
		if (COMMAND_FLASH_GET_VERSION == g_commandInstruction)
				SSPBUF = 0; //I'm in program so i dont have a bootloader version
		else
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

bool SendMessageToOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	bool retVal;

#ifdef FOUR_PIN_INTERFACE
	INPUT_SWITCH = 0;
	OUTPUT_SWITCH = 1;
#else
	INnOUT_PORT = 0;
#endif

	I2cMasterInit();
	retVal = I2cMasterPut(messageType, command, data, count);
	I2cSlaveInit();

#ifdef FOUR_PIN_INTERFACE
	OUTPUT_SWITCH = 0;
	INPUT_SWITCH = 1;
#else
	INnOUT_PORT = 1;
#endif

	return retVal;
}

unsigned char GetMessageFromOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	unsigned char value;

#ifdef FOUR_PIN_INTERFACE
	INPUT_SWITCH = 0;
	OUTPUT_SWITCH = 1;
#else
	INnOUT_PORT = 0;
#endif

	I2cMasterInit();
	value = I2cMasterGet(messageType, command, data, count);
	I2cSlaveInit();

#ifdef FOUR_PIN_INTERFACE
	OUTPUT_SWITCH = 0;
	INPUT_SWITCH = 1;
#else
	INnOUT_PORT = 1;
#endif

	return value;
}

void SendToOutputIfReady()
{
	if (g_toOutput.isReady)
	{
		if (TO_OUTPUT_MAX_TRAY == g_toOutput.try)
		{
			g_toOutput.isReady = false;
			//TODO: solve module on output is not connected an more
			return;
		}
		g_toOutput.try = g_toOutput.try + 1;
		if (g_toOutput.isState)
		{
			if (SendMessageToOutput(I2C_MESSAGE_TYPE_DATA, 0, &g_state, 1))
				g_toOutput.isReady = false;
			else //message was not send
				INVERT_OUTPUT_PORT = !INVERT_OUTPUT_PORT; //output device may be connected by an oposite way
		}
		else
		{
			//TODO: command send
		}

	}
}