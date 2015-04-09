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
#include <QVBoxLayout>
#include <vector>

BootLoader::BootLoader(QWidget *parent) :
	QWidget(parent),
	m_hexPath(NULL),
	m_textEdit(NULL),
	m_directory("/")
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
	connect(hexButton, SIGNAL(clicked()), this, SLOT(upload()));

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
		std::vector<unsigned> words(2048, 0x3fff);

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
						if (address + i >= words.size())
						{
							QMessageBox::critical(this, "", tr("address out of range."));
							return;
						}
						words[address + i] = value;
					}
					break;
				case 1: //end of file
					break;
				default:
				 QMessageBox::critical(this, "", tr("unsuported record type."));
			 }
		}

		m_textEdit->clear();
		for (int counter = 0; counter <  words.size(); counter += 8)
		{
			 QString outLine = QString("%1 | 0x%2, 0x%3, 0x%4, 0x%5, 0x%6, 0x%7, 0x%8, 0x%9,").
				arg(counter, 4, 16, QChar('0')).
				arg(words[counter], 4, 16, QChar('0')).
				arg(words[counter+1], 4, 16, QChar('0')).
				arg(words[counter+2], 4, 16, QChar('0')).
				arg(words[counter+3], 4, 16, QChar('0')).
				arg(words[counter+4], 4, 16, QChar('0')).
				arg(words[counter+5], 4, 16, QChar('0')).
				arg(words[counter+6], 4, 16, QChar('0')).
				arg(words[counter+7], 4, 16, QChar('0'));
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

}
