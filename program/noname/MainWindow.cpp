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
#include <QThread>
#include <Plot.h>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_serialPort(NULL)
{
	setMinimumSize(400, 300);

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
			//_AddRgbTab();
			_AddPlotTab();
		}
		_AddBootloaderTab();
	}
}

void MainWindow::_SetMainLayout()
{
	m_tabWidget = new QTabWidget(this);
	this->setCentralWidget(m_tabWidget);
}

char const *MainWindow::_GetModuleTypeName(ModuleTypes type)
{
	switch(type)
	{
	case TYPE_USB_INTERFACE:
		return tr("USB Interface").toStdString().c_str();
	case TYPE_POTENTIOMETER:
		return tr("Potentiometer").toStdString().c_str();
	case TYPE_RGB_LED:
		return tr("RGB LED").toStdString().c_str();
	case TYPE_BUTTON:
		return tr("Button").toStdString().c_str();
	case TYPE_ARITHMETIC_LOGIC:
		return tr("Arithmetic-logic").toStdString().c_str();
	default:
		return tr("Unknown module").toStdString().c_str();
	}
}

bool MainWindow::_AddInterfaceTab()
{
	QWidget *widget = new QWidget(this);
	m_tabWidget->addTab(widget, _GetModuleTypeName((ModuleTypes)m_serialPort->GetModuleType(0)));
	QVBoxLayout *layout = new QVBoxLayout(widget);

	QSlider *slider = new QSlider(Qt::Horizontal, widget);
	slider->setMinimumSize(200, 20);
	slider->setRange(0, 1024);

	int value;
	if (m_serialPort->GetState(0, value))
	{
		slider->setValue(value);
		qDebug() << "value: " << value;
	}
	else
	{
		QMessageBox::critical(this, "", tr("Value was not set."));
	}

	connect(slider, SIGNAL(valueChanged(int)), m_serialPort, SLOT(SetValue(int)));
	layout->addWidget(slider);

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

void MainWindow::_AddPlotTab()
{
	Plot *plot = new Plot(this, m_serialPort);
	m_tabWidget->addTab(plot, _GetModuleTypeName((ModuleTypes)m_serialPort->GetModuleType(1)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::modeChanged(int value)
{
	m_serialPort->SetMode(value+1);

}
