#include "SerialPort.h"
#include <stdexcept>
#include <QThread>
#include <QtCore/QDebug>

#define BUFFER_SIZE 100
#define TIMEOUT 500

static char g_buffer[BUFFER_SIZE];

SerialPort::SerialPort(QObject *parent) :
	QObject(parent),
	m_serialPort(),
	m_timer(),
	m_dataReady(false),
	m_timeout(false)
{
	connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

SerialPort::~SerialPort()
{

}

void SerialPort::timerTimeout()
{
	m_timeout = true;
}

void SerialPort::readyRead()
{
	m_dataReady = true;
}

bool SerialPort::Open()
{
	if (!_OpenIfMyPotr())
		return false;

	return true;
}

void SerialPort::SetValue(int value)
{
	g_buffer[0] = value;
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, FID_SET_STATE, 1, 1, false);
	qDebug() << "value has been set to: " << value;
}

int SerialPort::GetMode()
{
	unsigned size = _CallCubeFunction(INTERFACE_MODULE_ADDRESS, FID_GET_MODE, 0, 1, true);
	if (0 == size || size != g_buffer[0])
		return 0;

	qDebug() << "mode: " << (int)(g_buffer[1]);
	return g_buffer[1];
}

void SerialPort::SetMode(int mode)
{
	g_buffer[0] = mode;
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, FID_SET_MODE, 1, 1, false);
}

bool SerialPort::FillValue(int &value)
{
	if (!_CallCubeFunction(INTERFACE_MODULE_ADDRESS, FID_GET_STATE, 0, 1, false))
			return false;

		value = (unsigned char)g_buffer[0];
		return true;
}


bool SerialPort::_OpenIfMyPotr()
{

	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		if (info.manufacturer() == "Microchip Technology, Inc.")
		{
			m_serialPort.setPortName(info.portName());
			m_serialPort.setBaudRate(115200);
			if (m_serialPort.open(QIODevice::ReadWrite))
			{
				if (_IsMyDevice())
					return true;

				m_serialPort.close();
			}
		}
	}

	return false;
}

bool SerialPort::_IsMyDevice()
{
	unsigned size = _CallCubeFunction(INTERFACE_MODULE_ADDRESS, FID_GET_PROTOCOL_ID, 0, 0, true);

	size_t pidSize = strlen(PROTOCOL_ID);
	if (size != MESSAGE_LENGTH_BYTE_COUNT + pidSize || size != (unsigned)g_buffer[0])
		return false;

	return (0 == strncmp(PROTOCOL_ID, g_buffer + MESSAGE_LENGTH_BYTE_COUNT, pidSize));
}

unsigned SerialPort::_CallCubeFunction(
		char moduleId, FunctionId functionId, unsigned inDataSize, unsigned requiredSize, bool usbCubeLookingFor)
{
	if (!m_serialPort.isOpen())
		throw std::runtime_error(tr("Hardware is unavailable. Please check if it is connected or another program instance is not running.").toStdString());

	char messageLength = 3 + inDataSize; //1x messgeLength + 1x moduleId +1x functionId

	std::string message;
	message.append((char *)&messageLength, 1);
	message.append((char*)&moduleId, 1);
	message.append((char*)&functionId, 1);
	message.append(g_buffer, inDataSize);

	m_serialPort.write(message.c_str(), messageLength);
	m_serialPort.waitForBytesWritten(1000);

	int size = _ReadData();

	if
	(
		!usbCubeLookingFor &&
		(size == 0 || (requiredSize != 0 && size != requiredSize) || (requiredSize == 0 && size != (unsigned)g_buffer[0]))
	)
		throw std::runtime_error(tr("Readed data are uncomplete.").toStdString());

	return size;

}

unsigned SerialPort::_ReadData()
{
	m_serialPort.waitForReadyRead(100);
	QByteArray data = m_serialPort.readAll();

	if (data.size() > 0)
	{
		strcpy(g_buffer, data.toStdString().c_str());
		//qDebug() << "readed: " << g_buffer;
	}
	return data.size();
}

