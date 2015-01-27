#ifndef __I2C_CONNECTOR_H__
#define __I2C_CONNECTOR_H__

#define DEFAULT_ADDRESS 0x1

void I2CInit(void);
void PutI2C(unsigned char address, unsigned char *data, unsigned char count, unsigned char sendStop);
unsigned GetI2C(int address, unsigned char sendStart);
unsigned char PutAndGetI2C(
    unsigned char address, unsigned char *data, unsigned char count);
//unsigned char FillFromI2C(int address, char *data);


#endif //__I2C_CONNECTOR_H__