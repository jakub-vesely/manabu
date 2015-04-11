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
	int GetMode();
	int GetFlashVersion();
	void SetFlashAddress(uint16_t address);
	bool SetFlashWriteWord(uint16_t word);
	bool SetFlashLatchWord(uint16_t word);

	void SetMode(int mode);
	bool FillValue(int &value);
protected slots:
	void readyRead();
	void timerTimeout();

};

#endif // SERIALPORT_H
