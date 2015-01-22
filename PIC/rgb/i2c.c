#include "i2c.h"

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
			if (SEN)
				CKP = 1;           //release the SCL line
		break;

		//I2C write, last byte was data
		//SSPSTAT bits D/A=1, S=1, R/W=0, BF=1
		case 0b00101001:
			value = SSPBUF; //read buffer, clear BF
			if (SEN)
				CKP = 1;

			if (value != g_value)
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
