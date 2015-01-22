//inspired by http://www.microchip.com/forums/m677165.aspx

#ifndef _COMMON_I2C_H_
#define _COMMON_I2C_H_

#include <common/common.h>

#define TRIS_SCL TRISCbits.TRISC0 
#define TRIS_SDA TRISCbits.TRISC1 

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

void ProcessI2cInterrupt(unsigned char status);

#endif //_COMMON_I2C_H_
