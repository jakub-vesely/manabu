#ifndef __I2C_CONNECTOR_H__
#define __I2C_CONNECTOR_H__

#include <common/common.h>

void I2CInit(void);
void PutStateI2C(unsigned char state);
void PutCommandI2C(I2cCommand command, unsigned char *data, unsigned char count);
unsigned char GetCommandI2C(I2cCommand command);

void PutI2C(unsigned char messageType, I2cCommand command, unsigned char *data, unsigned char count);
unsigned char GetI2C(unsigned char messageType, I2cCommand command, unsigned char *data, unsigned char count);



#endif //__I2C_CONNECTOR_H__