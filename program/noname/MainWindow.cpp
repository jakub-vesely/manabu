#include "MainWindow.h"
#include "ui_MainWindow.h"
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
	serialPort->Open();

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
	connect(slider, SIGNAL(valueChanged(int)), serialPort, SLOT(SetValue(int)));

}

MainWindow::~MainWindow()
{
}
