#include "SerialPort.h"
#include <stdexcept>
#include <QMessageBox>
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
	return _OpenIfMyPotr();
}

bool SerialPort::SetValue(int value)
{
	memcpy(g_buffer, (char const *)&value, 2);
	if (!_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_SET_STATE, 2, 2, false))
		return false;

	qDebug() << "value has been set to: " << value;
	return true;
}

bool SerialPort::GetState(unsigned layer, int &value)
{
	if (!_CallCubeFunction(layer, MID_GET_STATE, 0, 3, false))
			return false;

		value = *(uint16_t *)(g_buffer+1);
		qDebug() << "get value: " << *(uint16_t *)(g_buffer+1);
		return true;
}

int SerialPort::GetMode()
{
/*	unsigned size = _CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_GET_MODE, 0, 1, true);
	if (0 == size || size != g_buffer[0])
		return 0;

	qDebug() << "mode: " << (int)(g_buffer[1]);
	return g_buffer[1];*/
	return 0;
}

int SerialPort::GetFlashVersion1()
{
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_COMMAND_FLASH_GET_VERSION, 0, 2, true);

	qDebug() << "flash version: " << (int)(g_buffer[1]);
	return g_buffer[1];
}

void SerialPort::SetFlashAddress(uint16_t address)
{
	((uint16_t *)g_buffer)[0] = address;
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_COMMAND_FLASH_ADDRESS, 2, 2, true);
}

void SerialPort::SetFlashWriteWord(uint16_t word)
{
	((uint16_t *)g_buffer)[0] = word;
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_COMMAND_FLASH_WRITE_WORD, 2, 2, true);
}

void SerialPort::SetFlashLatchWord(uint16_t word)
{
	((uint16_t *)g_buffer)[0] = word;
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_COMMAND_FLASH_LATCH_WORD, 2, 2, true);
}

int SerialPort::GetFlashCheckSum()
{
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_COMMAND_FLASH_CHECKSUM, 0, 2, true);
	return g_buffer[1];
}

void SerialPort::SetFlashEnd()
{
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_COMMAND_FLASH_END, 0, 2, true);
}

void SerialPort::SetFlashLoadCheck(unsigned char byte)
{
	g_buffer[0] = byte;
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_COMMAND_FLASH_SET_BOOT_FLAG, 1, 2, true);
}

void SerialPort::SetMode(int mode)
{
/*	g_buffer[0] = mode;
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_SET_MODE, 1, 1, false);
	*/
}

bool SerialPort::FillModuleType(unsigned layer, ModuleTypes &moduleType)
{
	if (!_CallCubeFunction(layer, MID_GET_MODULE_TYPE, 0, 2, false))
		return false;

	qDebug() << "module type on layer:" << layer << " is " << (unsigned char)(g_buffer[1]);
	moduleType = (ModuleTypes)g_buffer[1];

	return true;
}


bool SerialPort::_OpenIfMyPotr()
{

	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		if (info.manufacturer() == "Microchip Technology, Inc.")
		{
			qDebug() << info.portName() << "looks like my port";
			m_serialPort.setPortName(info.portName());
			m_serialPort.setBaudRate(115200);
			if (m_serialPort.open(QIODevice::ReadWrite))
			{
				if (_IsMyDevice())
				{
					qDebug() << info.portName() << "is my port";
					return true;
				}
				else
				{
					qDebug() << info.portName() << "is not my port";
				}
				m_serialPort.close();
			}
			else
			{
				qDebug() << info.portName() << "was not open";
			}
		}
	}

	return false;
}

bool SerialPort::_IsMyDevice()
{
	qDebug() << "is really my device?";
	size_t pidSize = strlen(PROTOCOL_ID);

	if (!_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_GET_PROTOCOL_ID, 0, pidSize+1, false))
		return false;

	return (0 == strncmp(PROTOCOL_ID, g_buffer + MESSAGE_LENGTH_BYTE_COUNT, pidSize));
}

bool SerialPort::_CallCubeFunction(char moduleId, MessageId functionId, unsigned inDataSize, unsigned expectedSize, bool forceSize)
{
	if (!m_serialPort.isOpen())
	{
		qDebug() << "Error: Hardware is unavailable. Please check if it is connected or another program instance is not running.";
		throw std::runtime_error(tr("Hardware is unavailable. Please check if it is connected or another program instance is not running.").toStdString());
	}
	char messageLength = 3 + inDataSize; //1x messgeLength + 1x moduleId +1x functionId

	std::string message;
	message.append((char *)&messageLength, 1);
	message.append((char*)&moduleId, 1);
	message.append((char*)&functionId, 1);
	message.append(g_buffer, inDataSize);

	m_serialPort.write(message.c_str(), messageLength);
	m_serialPort.waitForBytesWritten(50);

	int size = _ReadData();

	if(
		size == 0 ||
		(expectedSize != 0 && size != expectedSize) ||
		(expectedSize == 0 && size != (unsigned)g_buffer[0])
	)
	{
		qDebug() << "Error: Readed data are uncomplete. required_size:" << expectedSize << "size:" << size << " g_buffer[0]:" << (unsigned)g_buffer[0];

		if (forceSize)
			throw std::runtime_error(tr("Readed data are uncomplete.").toStdString());
		else return false;
	}
	return true;

}

unsigned SerialPort::_ReadData()
{
	m_serialPort.waitForReadyRead(50);
	QByteArray data = m_serialPort.readAll();

	if (data.size() > 0)
		strcpy(g_buffer, data.toStdString().c_str());

	return data.size();
}

