#include "MainWindow.h"
#include <QMessageBox>
#include "SerialPort.h"
#include <QString>
#include <QSlider>
#include <QVBoxLayout>
#include <QtCore/QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	setMinimumSize(200, 20);
	SerialPort *serialPort = new SerialPort(this);
	if (!serialPort->Open())
	{
		QMessageBox::critical(this, "", tr("Device not connected."));
		return;
	}

	QVBoxLayout *vBoxLayout = new  QVBoxLayout(this);
	setLayout(vBoxLayout);

	QSlider *slider = new QSlider(Qt::Horizontal, this);
	slider->setMinimumSize(200, 20);
	vBoxLayout->addWidget(slider);

	slider->setRange(0, 255);

	int value;
	if (serialPort->FillValue(value))
	{
		slider->setValue(value);
		qDebug() << "value: " << value;
	}
	else
	{
		QMessageBox::critical(this, "", tr("Value was not read."));

	}
	connect(slider, SIGNAL(valueChanged(int)), serialPort, SLOT(SetValue(int)));

}

MainWindow::~MainWindow()
{
}
