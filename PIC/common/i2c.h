//inspired by http://www.microchip.com/forums/m677165.aspx

#ifndef _COMMON_I2C_H_
#define _COMMON_I2C_H_

#include <common/common.h>
#include <common/common_16F1503.h> 

void I2cSlaveInit();
void I2cMasterInit(void);
void I2cMasterStart(void);
bool I2cMasterWrite(char byte);
void I2cMasterStop(void);
bool I2cMasterPut(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count);
unsigned char I2cMasterGet(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count);

void CheckI2C();



#endif //_COMMON_I2C_H_
