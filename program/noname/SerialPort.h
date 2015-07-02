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
	bool _CallCubeFunction(char moduleId, MessageId functionId, unsigned inDataSize, unsigned expectedSize, bool forceSize);
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
	bool SetValue(int value);
	int GetMode();
	int GetFlashVersion1();
	void SetFlashAddress(uint16_t address);
	void SetFlashWriteWord(uint16_t word);
	void SetFlashLatchWord(uint16_t word);
	int GetFlashCheckSum();
	void SetFlashEnd();
	void SetFlashLoadCheck(unsigned char byte);
	void SetMode(int mode);
	bool FillModuleType(unsigned layer, ModuleTypes &moduleType);
	bool GetState(unsigned layer, int &value);
protected slots:
	void readyRead();
	void timerTimeout();

};

#endif // SERIALPORT_H
