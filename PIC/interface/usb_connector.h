#ifndef __USB_CONNECTOR_H__
#define __USB_CONNECTOR_H__

void InitializeUSB(void);
void ProcessUSB(void (*usbDataReaded)(unsigned char const *data, int size));
void PutUsbData(char *data, char size);
#endif //__USB_CONNECTOR_H_