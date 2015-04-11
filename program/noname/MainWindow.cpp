#include "MainWindow.h"
#include "SerialPort.h"
#include "BootLoader.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSlider>
#include <QString>
#include <QMessageBox>
#include <QTabWidget>
#include <QtCore/QDebug>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_serialPort(NULL)
{
	setMinimumSize(200, 20);

	_SetMainLayout();

	m_serialPort = new SerialPort(this);
	if (!m_serialPort->Open())
	{
		QMessageBox::critical(this, "", tr("Device not connected."));

	}
	else
	{
		if (0 != m_serialPort->GetFlashVersion())
		{
			qDebug() << "module with a bootloader connected";
		}
		else
		{
			_AddInterfaceTab();
			_AddRgbTab();
		}
		_AddBootloaderTab();
	}
}

void MainWindow::_SetMainLayout()
{
	m_tabWidget = new QTabWidget(this);
	this->setCentralWidget(m_tabWidget);
}

bool MainWindow::_AddInterfaceTab()
{


	QWidget *widget = new QWidget(this);
	m_tabWidget->addTab(widget, tr("Interface"));
	QVBoxLayout *layout = new QVBoxLayout(widget);

	QSlider *slider = new QSlider(Qt::Horizontal, widget);
	slider->setMinimumSize(200, 20);
	slider->setRange(0, 255);
	connect(slider, SIGNAL(valueChanged(int)), m_serialPort, SLOT(SetValue(int)));
	layout->addWidget(slider);


	int value;
	if (m_serialPort->FillValue(value))
	{
		slider->setValue(value);
		qDebug() << "value: " << value;
	}
	else
	{
		QMessageBox::critical(this, "", tr("Value was not set."));
	}

	return true;
}

void MainWindow::_AddRgbTab()
{
	QWidget *widget = new QWidget(this);
	m_tabWidget->addTab(widget, tr("RGB"));
	QVBoxLayout *layout = new QVBoxLayout(widget);

	QComboBox *mode = new QComboBox(widget);
	mode->addItem(tr("red to red"));
	mode->addItem(tr("red to purple"));
	mode->addItem(tr("white value"));
	mode->setCurrentIndex(m_serialPort->GetMode()-1);

	connect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(modeChanged(int)));
}

void MainWindow::_AddBootloaderTab()
{
	BootLoader *bootLoader = new BootLoader(this, m_serialPort);
	m_tabWidget->addTab(bootLoader, tr("Bootloader"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::modeChanged(int value)
{
	m_serialPort->SetMode(value+1);

}
