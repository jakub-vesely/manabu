#include "BootLoader.h"
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
#include <LogDialog.h>

BootLoader::BootLoader(QWidget *parent, SerialPort *serialPort, bool layer, LogDialog *logDialog) :
	QDialog(parent),
	m_hexPath(NULL),
	m_hexView(NULL),
	m_directory("/GitRepository/stavebnice03/PIC"),
	m_serialPort(serialPort),
	m_words(0x2000, 0x3fff),
	m_layer(layer),
	m_uploadButton(NULL),
	m_log(logDialog)

{
	QVBoxLayout *layout = new QVBoxLayout(this);

	if (0 == layer)
	{
		QPushButton *goToBootloaderButton = new QPushButton(this);
		goToBootloaderButton->setText(tr("Erase main program and go to bootloader "));
		layout->addWidget(goToBootloaderButton);
		connect(goToBootloaderButton, SIGNAL(clicked()), this, SLOT(goToBootloader()));
		return;
	}

	QHBoxLayout *hexPathLaout = new QHBoxLayout(this);
	layout->addLayout(hexPathLaout);
	m_hexPath = new QLineEdit(this);
	hexPathLaout->addWidget(m_hexPath);
	QPushButton *hexButton = new QPushButton(this);
	hexButton->setText("...");
	hexButton->setFixedWidth(30);
	hexPathLaout->addWidget(hexButton);
	connect(hexButton, SIGNAL(clicked()), this, SLOT(openHex()));

	m_uploadButton = new QPushButton(this);
	m_uploadButton->setText(tr("Upload"));
	m_uploadButton->setDisabled(true);
	hexPathLaout->addWidget(m_uploadButton);
	connect(m_uploadButton, SIGNAL(clicked()), this, SLOT(upload()));

	m_hexView = new QTextEdit(this);
	QFont font(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	font.setPointSize(10);
	m_hexView->setFont(font);
	m_hexView->setMinimumWidth(400);
	m_hexView->setReadOnly(true);
	layout->addWidget(m_hexView);

	m_status = new QTextEdit(this);
	m_status->setReadOnly(true);
	m_status->setFixedHeight(100);
	layout->addWidget(m_status);

}

BootLoader::~BootLoader()
{

}

void BootLoader::goToBootloader()
{
	if (QMessageBox::Yes == QMessageBox::question(this, "", tr("Are you sure you want to remove just running program from a connected interface module?")))
	{
			m_serialPort->SetFlashLoadCheck(0, 0xff);
			QMessageBox::information(this, "", tr("Turn off the application, connect interface module to anothe one and turn on the application again."));
	}

}

void BootLoader::openHex()
{
	const unsigned wordSize = 2;
	QString fileName;
	fileName = QFileDialog::getOpenFileName(this,
		 tr("Open Intel-Hex file"), m_directory, tr("Intel-Hex file (*.hex)"));

	if ("" == fileName)
		return;

	m_uploadButton->setDisabled(true);
	m_directory = QFileInfo(fileName).absoluteDir().absolutePath();
	m_status->append(QString(tr("loaded HEX file:%1 has been loaded")).arg(fileName));
	m_hexPath->setText(fileName);

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);

		unsigned segment = 0;
		unsigned lastAddress = 0;
		while(!stream.atEnd())
		{
			QString inLine = stream.readLine();
			//m_textEdit->append(line);
			unsigned dataLength = inLine.mid(1, 2).toInt(0, 16) /wordSize;
			unsigned address = segment + inLine.mid(3, 4).toInt(0, 16) / wordSize;
			unsigned type =  inLine.mid(7, 2).toInt(0, 16);

			//qDebug() << "processed address:" << address;
			switch (type)
			{
				case 0: //data
					for (int i = 0; i < dataLength; i++)
					{
						unsigned value = inLine.mid(9 + i*4, 2).toInt(0, 16);
						value += (inLine.mid(9 + i*4 + 2, 2).toInt(0, 16)) << 8;
						if (address + i < m_words.size()) //configuration flag will not be stored
						{
							m_words[address + i] = value;
							lastAddress = address + i;
						}
					}
					break;
				case 1: //end of file
					break;
				case 4:
					segment = 0x10000;
					break;
				default:
					QMessageBox::critical(this, "", tr("unsuported record type."));
			}
			m_uploadButton->setEnabled(true);
		}
		m_status->append(QString(tr("HEX file range 0x0100:0x%1")).arg(lastAddress, 4, 16, QChar('0')));
		m_status->repaint();
		m_words.resize(lastAddress + 1);

		m_hexView->clear();
		for (int counter = 0x100; counter <  m_words.size(); counter += 8)
		{
			 QString outLine = QString("%1 | %2 %3 %4 %5 %6 %7 %8 %9").
				arg(counter, 4, 16, QChar('0')).
				arg(m_words[counter], 4, 16, QChar('0')).
				arg(m_words[counter+1], 4, 16, QChar('0')).
				arg(m_words[counter+2], 4, 16, QChar('0')).
				arg(m_words[counter+3], 4, 16, QChar('0')).
				arg(m_words[counter+4], 4, 16, QChar('0')).
				arg(m_words[counter+5], 4, 16, QChar('0')).
				arg(m_words[counter+6], 4, 16, QChar('0')).
				arg(m_words[counter+7], 4, 16, QChar('0'));
			 m_hexView->append(outLine);
		}
	}
	//QTextCursor cursor;
	//cursor.setPosition(0);
	//m_textEdit->setTextCursor(cursor);
	m_hexView->moveCursor(QTextCursor::Start) ;

}

void BootLoader::upload()
{
	try
	{
		m_serialPort->Flashing(true);
		unsigned version = m_serialPort->GetFlashVersion(m_layer);
		if (0 == version)
		{
			m_status->append(tr("a program already present - going to bootloader..."));
			m_status->repaint();
			m_serialPort->SetFlashLoadCheck(1, 0xff);
			QThread::msleep(500); //wait for reset
		}
		else if (~0 == version)
		{
			m_status->append(tr("no module connected - programming terminated."));
			m_status->repaint();
			return;

		}
		else if (2 == version)
		{
			m_status->append(tr("found interface bootloader"));
			m_status->repaint();
		}
		else
		{
			m_status->append(tr("found common bootloader"));
			m_status->repaint();
		}

		m_status->append(tr("programing started..."));
		m_status->repaint();

		m_serialPort->GetFlashCheckSum(); //I dont care about checksum, just want to reser checksum in a bootloader

		unsigned char checkSum = 0;
		unsigned flashingInterface = (version == 2);
		unsigned rowSize = (flashingInterface ? 32 : 16);

		for (uint16_t address = 0x100; address <  m_words.size(); address++)
		{
			if (0 == (address % rowSize))
			{
				m_serialPort->SetFlashAddress(address);
				checkSum += address & 0xff;
				checkSum += address >> 8;
			}

			if (address != m_words.size()-1 && ((address + 1) % rowSize))
			{
				m_serialPort->SetFlashLatchWord(m_words[address]);

				checkSum += m_words[address] & 0xff;
				checkSum += m_words[address] >> 8;
			}
			else
			{
				m_serialPort->SetFlashWriteWord(m_words[address]);

				checkSum += m_words[address] & 0xff;
				checkSum += m_words[address] >> 8;

			}
		}

		unsigned char deviceCheckSum = m_serialPort->GetFlashCheckSum();
		if (checkSum == deviceCheckSum)
		{
			m_status->append(tr("checksum match"));
			m_status->repaint();

			if (flashingInterface)
			{
				QMessageBox::information(this, "", tr("Interface programming finihed. Disconnect the module, press reset button and then press OK."));
				m_serialPort->Flashing(false);
				m_status->append(tr("interface module programming finished"));
				m_status->repaint();
			}
			else
			{
				m_serialPort->SetFlashEnd();

				QThread::msleep(500); //wait for oscilator is stable

				unsigned version = m_serialPort->GetFlashVersion(m_layer);
				if (0 == version)
					m_status->append(tr("the program is runnimg"));
				else
					m_status->append(tr("the program DOESN'T runnimg"));

				m_status->repaint();
				m_serialPort->SetFlashLoadCheck(1, 0);

				m_serialPort->Flashing(false);

				m_status->append(tr("programming finished"));
				m_status->repaint();
			}
		}
		else
		{
			m_serialPort->Flashing(false);
			m_status->append(tr("Checksum doesn't match, unplug a device and plug it again and try it again"));
			m_status->repaint();
			m_log->Error(
				QString("checksum doesn't match. my checksum is: %1, device checksum is: %2").
					arg(checkSum).
					arg((unsigned char) deviceCheckSum)
			);
		}
	}
	catch (...)
	{
		m_status->append(tr("uploading error - module was disconnected"));
	}
	flashed();
}
