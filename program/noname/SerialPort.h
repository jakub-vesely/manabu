#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <stdint.h>

#include "../../CommonConstants.h"

class LogDialog;
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
	LogDialog *m_log;
public:
	SerialPort(QObject *parent, LogDialog *logDialog);
	bool Open();

	void Close()
	{ m_serialPort.close(); }

	void Flashing(bool active);

	bool IsOpen()
	{ return m_serialPort.isOpen(); }

	~SerialPort();

signals:

public slots:
	bool SetValue(int value);
	int GetFlashVersion(unsigned layer);
	void SetFlashAddress(uint16_t address);
	void SetFlashWriteWord(uint16_t word);
	void SetFlashLatchWord(uint16_t word);
	int GetFlashCheckSum();
	void SetFlashEnd();
	void SetFlashLoadCheck(unsigned layer, unsigned char byte);
	bool SetMode(unsigned layer, unsigned mode);
	bool FillModuleType(unsigned layer, ModuleTypes &moduleType);
	bool GetState(unsigned layer, int &value);
	int GetMode(unsigned layer, unsigned &mode);
protected slots:
	void readyRead();
	void timerTimeout();

};

#endif // SERIALPORT_H
