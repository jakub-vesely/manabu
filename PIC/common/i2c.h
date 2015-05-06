//inspired by http://www.microchip.com/forums/m677165.aspx

#ifndef _COMMON_I2C_H_
#define _COMMON_I2C_H_

#include <common/common.h>
#include <system_common.h>

void I2cSlaveInit();
void I2cMasterInit(void);
bool I2cMasterStart(void);
bool I2cMasterWrite(char byte);
bool I2cMasterStop(void);
bool I2cMasterPut(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count);
unsigned char I2cMasterGet(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count);

void CheckI2cAsSlave(void);

bool SendMessageToOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count);
unsigned char GetMessageFromOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count);
void SendToOutputIfReady();

#define  I2C_COMMON_INIT\
    SSPEN = 0;\
    PORTCbits.RC0 = 0;\
    PORTCbits.RC1 = 0;\
    TRISCbits.TRISC0 = 1;\
    TRISCbits.TRISC1 = 1;

#define I2C_SLAVE_SPECIFIC_INIT \
    SSPCON2 = 0b00000001; /*SEN is set to enable clock stretching*/\
    SSPCON3 = 0x00; \
    SSPMSK = 0x00; /*all address bits will be ignored*/\
    SSPADD = 0x00; /*no address is set, all the time is conneceted only one slave*/\
    SSPSTAT = 0x00; \
    SSPCON1 = 0b00010110; /*clock stretching + 7-bit addressing*/ \
    SSPEN = 1;  /*enable SSP and configures SDA & SCL pins*/

#endif //_COMMON_I2C_H_
