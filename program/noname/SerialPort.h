#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <stdint.h>

#include "../../CommonConstants.h"

class SerialPort : public QObject
{
	Q_OBJECT

	bool _OpenIfMyPotr();
	bool _IsMyDevice();
	unsigned _CallCubeFunction(
		char moduleId, FunctionId functionId, unsigned inDataSize, unsigned requiredSize, bool usbCubeLookingFor);
	unsigned _ReadData();

	QSerialPort m_serialPort;
	QTimer m_timer;
	bool m_dataReady;
	bool m_timeout;
public:
	explicit SerialPort(QObject *parent = 0);
	bool Open();

	~SerialPort();

signals:

public slots:
	void SetValue(int value);
	unsigned char GetValue(unsigned layer);
	int GetMode();
	int GetFlashVersion();
	void SetFlashAddress(uint16_t address);
	void SetFlashWriteWord(uint16_t word);
	void SetFlashLatchWord(uint16_t word);
	int GetFlashCheckSum();
	void SetFlashEnd();
	void SetFlashLoadCheck(unsigned char byte);
	void SetMode(int mode);
	unsigned char GetModuleType(unsigned layer);
	bool GetState(int &value);
protected slots:
	void readyRead();
	void timerTimeout();

};

#endif // SERIALPORT_H
