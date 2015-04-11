#include "BootLoader.h"
#include <QtCore/QDebug>
#include <QHBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTextStream>
#include <QThread>
#include <QVBoxLayout>
#include "SerialPort.h"
#include <vector>


BootLoader::BootLoader(QWidget *parent, SerialPort *serialPort) :
	QWidget(parent),
	m_hexPath(NULL),
	m_textEdit(NULL),
	m_directory("/"),
	m_serialPort(serialPort),
	m_words(2048, 0x3fff)

{
	QVBoxLayout *layout = new QVBoxLayout(this);

	QHBoxLayout *hexPathLaout = new QHBoxLayout(this);
	layout->addLayout(hexPathLaout);
	m_hexPath = new QLineEdit(this);
	hexPathLaout->addWidget(m_hexPath);
	QPushButton *hexButton = new QPushButton(this);
	hexButton->setText("...");
	hexButton->setFixedWidth(30);
	hexPathLaout->addWidget(hexButton);
	connect(hexButton, SIGNAL(clicked()), this, SLOT(openHex()));

	QPushButton *uploadButton = new QPushButton(this);
	uploadButton->setText(tr("Upload"));
	hexPathLaout->addWidget(uploadButton);
	connect(uploadButton, SIGNAL(clicked()), this, SLOT(upload()));

	m_textEdit = new QTextEdit(this);
	QFont font(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	font.setPointSize(10);
	m_textEdit->setFont(font);
	m_textEdit->setMinimumWidth(400);
	m_textEdit->setReadOnly(true);
	layout->addWidget(m_textEdit);
}

BootLoader::~BootLoader()
{

}

void BootLoader::openHex()
{
	const unsigned wordSize = 2;
	QString fileName = "C:/GitRepository/stavebnice03/program/arithmetic-logic.X.production.hex";
	fileName = QFileDialog::getOpenFileName(this,
		 tr("Open Intel-Hex file"), m_directory, tr("Intel-Hex file (*.hex)"));

	if ("" != fileName)
		m_directory = QFileInfo(fileName).absoluteDir().absolutePath();

	m_hexPath->setText(fileName);

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);

		while(!stream.atEnd())
		{
			 QString inLine = stream.readLine();
			 //m_textEdit->append(line);
			 unsigned dataLength = inLine.mid(1, 2).toInt(0, 16) /wordSize;
			 unsigned address = inLine.mid(3, 4).toInt(0, 16) / wordSize;
			 unsigned type =  inLine.mid(7, 2).toInt(0, 16);

			 switch (type)
			 {
				case 0: //data
					for (int i = 0; i < dataLength; i++)
					{
						unsigned value = inLine.mid(9 + i*4, 2).toInt(0, 16);
						value += (inLine.mid(9 + i*4 + 2, 2).toInt(0, 16)) << 8;
						if (address + i >= m_words.size())
						{
							QMessageBox::critical(this, "", tr("address out of range."));
							return;
						}
						m_words[address + i] = value;
					}
					break;
				case 1: //end of file
					break;
				default:
				 QMessageBox::critical(this, "", tr("unsuported record type."));
			 }
		}

		m_textEdit->clear();
		for (int counter = 0; counter <  m_words.size(); counter += 8)
		{
			 QString outLine = QString("%1 | 0x%2, 0x%3, 0x%4, 0x%5, 0x%6, 0x%7, 0x%8, 0x%9,").
				arg(counter, 4, 16, QChar('0')).
				arg(m_words[counter], 4, 16, QChar('0')).
				arg(m_words[counter+1], 4, 16, QChar('0')).
				arg(m_words[counter+2], 4, 16, QChar('0')).
				arg(m_words[counter+3], 4, 16, QChar('0')).
				arg(m_words[counter+4], 4, 16, QChar('0')).
				arg(m_words[counter+5], 4, 16, QChar('0')).
				arg(m_words[counter+6], 4, 16, QChar('0')).
				arg(m_words[counter+7], 4, 16, QChar('0'));
			 m_textEdit->append(outLine);
		}
	}
	//QTextCursor cursor;
	//cursor.setPosition(0);
	//m_textEdit->setTextCursor(cursor);
	m_textEdit->moveCursor(QTextCursor::Start) ;

}

void BootLoader::upload()
{
	if (0 != m_serialPort->GetFlashVersion())
	{
		qDebug() << "module with a bootloader connected";
	}
	QThread::msleep (40);

	unsigned version = m_serialPort->GetFlashVersion();
	if (0 == version)
		qDebug() << "a program already present";

	QThread::msleep (40);

	unsigned char checkSum = 0;
	for (uint16_t address = 0x100; address< m_words.size(); address++)
	{
		if (0 == address % 16)
		{
			qDebug() << "send address:" << address;
			m_serialPort->SetFlashAddress(address);
			checkSum += address & 0xff;
			checkSum += address >> 8;

			QThread::msleep (40);
		}

/*		if ((address-1) %16 || address = m_word.size()-1)
		{
			m_serialPort->SetFlashWriteWord(m_words[address]);
			QThread::msleep (4);
		}else
			m_serialPort->SetFlashLatchWord(m_words[address]);

		checkSum += m_words[address] & 0xff;
		checkSum += m_words[address] >> 8;
	*/
		/*
GetCommandI2C(COMMAND_FLASH_CHECKSUM, &slaveChecksum);
		if (checkSum == slaveChecksum)
			PORTC = 0b0100;
		PutCommandI2C(COMMAND_FLASH_END, NULL, 0);
		{
			counter = 0xfff;
			while (0 != --counter);
		}
		while (!GetCommandI2C(COMMAND_FLASH_GET_VERSION, &version));
		PORTC = 0b0101;
		if (0 == version)
		{
			unsigned char value = 0;
			PutCommandI2C(COMMAND_FLASH_LOAD_CHECK, value, 1);
			PORTC = 0b0111;
		}
*/

	}
}
