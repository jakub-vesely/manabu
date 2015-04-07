#ifndef __I2C_CONNECTOR_H__
#define __I2C_CONNECTOR_H__

#include <common/common.h>

void I2CInit(void);
unsigned char  PutStateI2C(unsigned char state);
unsigned char PutCommandI2C(I2cCommand command, unsigned char const *data, unsigned char count);
unsigned char GetCommandI2C(I2cCommand command);

unsigned char PutI2C(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count);
unsigned char GetI2C(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count);



#endif //__I2C_CONNECTOR_H__