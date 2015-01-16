#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

class SerialPort : public QObject
{
	Q_OBJECT

	enum FunctionId
	{
		FID_GET_PARAMETERS,
		FID_GET_PROTOCOL_ID,
		FID_GET_NEIGHBOR_ADDRESS,
		FID_GET_STATE,
		FID_SET_STATE,
		FID_SET_ADDRESS
	};

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
	bool FillValue(int &value);
protected slots:
	void readyRead();
	void timerTimeout();

};

#endif // SERIALPORT_H
