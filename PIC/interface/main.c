#include "i2c_connector.h"
#include "HardwareProfile.h"
#include "usb_connector.h"

#include <p18F14K50.h> 
#include <delays.h> 
#include <string.h>
#include <eep.h>

#include <CommonConstants.h>

//14K50
#pragma config CPUDIV = NOCLKDIV
#pragma config USBDIV = OFF
#pragma config FOSC   = HS
#pragma config PLLEN  = ON
#pragma config FCMEN  = OFF
#pragma config IESO   = OFF
#pragma config PWRTEN = OFF
#pragma config BOREN  = OFF
#pragma config BORV   = 30
#pragma config WDTEN  = OFF
#pragma config WDTPS  = 32768
#pragma config MCLRE  = OFF
#pragma config HFOFST = OFF
#pragma config STVREN = ON
#pragma config LVP    = OFF
#pragma config XINST  = OFF
#pragma config BBSIZ  = OFF
#pragma config CP0    = OFF
#pragma config CP1    = OFF
#pragma config CPB    = OFF
#pragma config WRT0   = OFF
#pragma config WRT1   = OFF
#pragma config WRTB   = OFF
#pragma config WRTC   = OFF
#pragma config EBTR0  = OFF
#pragma config EBTR1  = OFF
#pragma config EBTRB  = OFF 

#define OUTPUT_COUNT 5


#pragma udata
char out_buffer[8]; //FIXME: originally ther was 30 but something overwrited followed constants
const char protocolId[] = PROTOCOL_ID;
#pragma code

char state = 255;

int IsConnected()
{
	int i;

	if (!PORTBbits.RB7)
	{	
		return 0;
	}
	
	PORTBbits.RB7 = 0;
	for (i = 0; i < 100; i++);
	PORTBbits.RB7 = 1;
	
	return !PORTBbits.RB7;
}

void Response(char const * answer, unsigned answerLength)
{
	out_buffer[0] = 1 + answerLength;
	memcpy(out_buffer+1, answer, answerLength);
	PutUsbData(out_buffer, out_buffer[0]);	
}

/*void GetParameters(char const *data)
{
	unsigned address = data[1];
	char pos;
	out_buffer[1] = FIRMWARE_VERSION;
	if (INTERFACE_CUBE_ADDRESS == address)
	{
		out_buffer[2] = CT_INTERFACE;
		out_buffer[3] = OUTPUT_COUNT;
		pos = 4; //size + version + type size + outputCount
		memcpy(out_buffer + pos, (void *)serial_number, sizeof(serial_number)-1);
 		pos += sizeof(serial_number)-1;
		
	}
	else if (led1Address == address)
	{
		out_buffer[2] = CT_LED;
		out_buffer[3] = 0;
		pos = 4; //size + version + type size + outputCount
		memcpy(out_buffer + pos, (void *)led1SN, sizeof(led1SN)-1);
 		pos += sizeof(led1SN)-1;
	}
	else if (buttonAddress == address)
	{
		out_buffer[2] = CT_BUTTON;
		out_buffer[3] = 0;
		pos = 4; //size + version + type size + outputCount
		memcpy(out_buffer + pos, (void *)buttonSN, sizeof(buttonSN)-1);
 		pos += sizeof(buttonSN)-1;
	}
	else if (buttonAddress < address)
	{
		PutI2C(address, FID_GET_PARAMETERS);
		pos = FillFromI2C(address, out_buffer);				
	}

	out_buffer[0] = pos;
	PutUsbData(out_buffer, pos);
}

void GetNeighborAddress(char const *data)
{
	if (data[1] == INTERFACE_CUBE_ADDRESS)
	{
		switch (data[3])
		{
			case 0:
				out_buffer[0] = led1Address;
			break;
			case 1:
				out_buffer[0] = buttonAddress;
			break;
			case 2:
				PORTC = 0b1001;
				if (INIT_SLAVA_CUBE_ADDRESS != firstDeviceAddress && !PORTBbits.RB7) 
				//IsConnected was already called and device is still connected
				{
					out_buffer[0] = firstDeviceAddress;
					PORTC = 0b0011;
				}
				else if (IsConnected())
				{
					int i;
					PORTC = 0b0111;
					firstDeviceAddress = ++maxAddress;
					
					PutI2C(INIT_SLAVA_CUBE_ADDRESS, FID_SET_ADDRESS);
					PutI2C(INIT_SLAVA_CUBE_ADDRESS, firstDeviceAddress);

					//time to set the address 
					//may be it would be bater to solve it by holding clk 
					for (i = 0; i < 1000; i++);

					out_buffer[0] = firstDeviceAddress;					
				}
				else 
				{
					out_buffer[0] = 0;
					PORTC = 0b0101;
				}
			break;
			default:
				out_buffer[0] = 0;
			break;
		}
	}
	else
		out_buffer[0] = 0;

	PutUsbData(out_buffer, 1);
}
*/
void SetState(char const *data)
{
	state = data[3];
	Write_b_eep(0, state);
	Busy_eep ();

	PutI2C(0x00, state);
	out_buffer[0] = 1;
	PutUsbData(out_buffer, 1);
}

void GetState(char const *data)
{
	out_buffer[0] = state;
	PutUsbData(out_buffer, 1);
}

void usbDataReaded(char const *data, int size)
{
	FunctionId functionId;
	
	if (0 == size)
		return;

	functionId = data[2];
	switch (functionId)
	{
		case FID_GET_PROTOCOL_ID:
			Response(protocolId, sizeof(protocolId)-1);
			PORTC = 0b0011;
		break;

		case FID_GET_PARAMETERS:
			//GetParameters(data);
		break;
		
		case FID_GET_NEIGHBOR_ADDRESS:
			//GetNeighborAddress(data);
		break;
		case FID_SET_STATE:
			PORTC = 0b0111;
			SetState(data);
			
		break;

		case FID_GET_STATE:
			GetState(data);
		break;
	}
}
void main(void)
{  
	int pass = 0;

	LATC=0;
	TRISC = 0;

	I2CInit();
	InitializeUSB();

	LATB=0;
	LATBbits.LATB7= 0;
	TRISBbits.TRISB7 = 0; 
	PORTBbits.RB7 = 1; //is connected bite

	
	state = Read_b_eep(0);
	PutI2C(0x01, state);
	while(1)
    {
		
		
		ProcessUSB(usbDataReaded);  
    }//end while
}//end main
