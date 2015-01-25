//inspired by http://www.microchip.com/forums/m677165.aspx

#ifndef _COMMON_I2C_H_
#define _COMMON_I2C_H_

#include <common/common.h>

#define TRIS_SCL TRISCbits.TRISC0 
#define TRIS_SDA TRISCbits.TRISC1 

void I2cInit(unsigned char address);
void ProcessI2cInterrupt();


#endif //_COMMON_I2C_H_
