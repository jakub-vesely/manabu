#include "SerialPort.h"
#include <stdexcept>
#include <QMessageBox>
#include <QThread>
#include <LogDialog.h>
#define BUFFER_SIZE 100
#define TIMEOUT 500

static char g_buffer[BUFFER_SIZE];

SerialPort::SerialPort(QObject *parent, LogDialog *logDialog) :
	QObject(parent),
	m_serialPort(),
	m_timer(),
	m_dataReady(false),
	m_timeout(false),
    m_log(logDialog),
    m_foundClosedPort(false)
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

	m_log->Info(QString("value has been set to: %1").arg(value));
	return true;
}

bool SerialPort::GetState(unsigned layer, int &value)
{
	if (!_CallCubeFunction(layer, MID_GET_STATE, 0, 3, false))
			return false;

	value = *(uint16_t *)(g_buffer+1);
	m_log->Info(QString("get value: %1").arg(*(uint16_t *)(g_buffer+1)));
	return true;
}

int SerialPort::GetMode(unsigned layer, unsigned &mode)
{
	if (!_CallCubeFunction(layer, MID_GET_MODE, 0, 2, false))
			return false;

	mode = g_buffer[1];
	m_log->Info(QString("get mode: %1").arg(mode));
	return true;
}

int SerialPort::GetFlashVersion(unsigned layer)
{
	_CallCubeFunction(layer, MID_COMMAND_FLASH_GET_VERSION, 0, 2, true);

	m_log->Info(QString("flash version: %1").arg((int)(g_buffer[1])));
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

void SerialPort::SetFlashLoadCheck(unsigned layer, unsigned char byte)
{
	g_buffer[0] = byte;
	_CallCubeFunction(layer, MID_COMMAND_FLASH_SET_BOOT_FLAG, 1, 2, false); //I don't want to force a size because when I call it for the interface module will get no response
}

void SerialPort::Flashing(bool active)
{
	_CallCubeFunction(INTERFACE_MODULE_ADDRESS, active ? MID_INTERFACE_FLASHING_START : MID_INTERFACE_FLASHING_STOP, 0, 2, true); //I don't want to force a size because when I call it for the interface module will get no response
}

bool SerialPort::SetMode(unsigned layer, unsigned mode)
{
	g_buffer[0] = mode;
	m_log->Info(QString("mode on layer: %1 set to %2").arg(layer).arg(mode));

	return _CallCubeFunction(layer, MID_SET_MODE, 1, 2, false);
}

bool SerialPort::FillModuleType(unsigned layer, ModuleTypes &moduleType)
{
	if (!_CallCubeFunction(layer, MID_GET_MODULE_TYPE, 0, 2, false))
		return false;

	m_log->Info(QString("module type on layer: %1 is %2").arg(layer).arg((unsigned char)(g_buffer[1])));
	moduleType = (ModuleTypes)g_buffer[1];

	return true;
}


bool SerialPort::_OpenIfMyPotr()
{
    m_foundClosedPort = false;
	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
        if (info.manufacturer() == "Microchip Technology, Inc." || info.manufacturer() == "Microchip Technology Inc.")
		{
			m_log->Info(QString("%1 looks like my port").arg(info.portName()));
            m_serialPort.setPort(info);
            m_serialPort.setBaudRate(115200);

            //to open port in linux you have to permit access by
            //sudo chmod a+rw /dev/ttyACM0
			if (m_serialPort.open(QIODevice::ReadWrite))
			{
				if (_IsMyDevice())
				{
					m_log->Info(QString("%1 is my port").arg(info.portName()));
					return true;
				}
				else
				{
					m_log->Warning(QString("%1 is not my port").arg(info.portName()));
				}
				m_serialPort.close();
			}
			else
			{
                m_foundClosedPort = true;
				m_log->Error(QString("%1 was not opened").arg(info.portName()));
			}
		}
	}

	return false;
}

bool SerialPort::_IsMyDevice()
{
	size_t pidSize = strlen(PROTOCOL_ID);

	if (!_CallCubeFunction(INTERFACE_MODULE_ADDRESS, MID_GET_FIRMWARE_VERSION, 0, pidSize+1, false))
		return false;

	return (0 == strncmp(PROTOCOL_ID, g_buffer + MESSAGE_LENGTH_BYTE_COUNT, pidSize));
}

bool SerialPort::_CallCubeFunction(char moduleId, MessageId functionId, unsigned inDataSize, unsigned expectedSize, bool forceSize)
{
	if (!m_serialPort.isOpen())
	{
		m_log->Error(QString("Hardware is unavailable. Please check if it is connected or another program instance is not running."));
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
		m_log->Error(
			QString("Readed data are uncomplete. required_size: %1, size: %2, moduleId: %3 functionId: %4" ).
				arg(expectedSize).
				arg(size).
				arg((int)moduleId).
				arg(functionId)
		);

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

