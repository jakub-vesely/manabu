#include "MainWindow.h"
#include <QMessageBox>
#include "SerialPort.h"
#include <QString>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtCore/QDebug>
#include <QLineEdit>
#include <QTabWidget>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_serialPort(NULL)
{
	setMinimumSize(200, 20);

	_SetMainLayout();
	if (!_AddInterfaceTab())
		return;

	_AddRgbTab();
}

void MainWindow::_SetMainLayout()
{
	m_tabWidget = new QTabWidget(this);
	this->setCentralWidget(m_tabWidget);
}

bool MainWindow::_AddInterfaceTab()
{
	m_serialPort = new SerialPort(this);
	if (!m_serialPort->Open())
	{
		QMessageBox::critical(this, "", tr("Device not connected."));
		return false;
	}

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
		QMessageBox::critical(this, "", tr("Value was not read."));

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

	//m_serialPort->SetMode(mode->currentIndex()+1);

	connect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(modeChanged(int)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::modeChanged(int value)
{
	m_serialPort->SetMode(value+1);

}
