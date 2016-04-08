#include "Interface.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QMessageBox>
#include <SerialPort.h>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>

Interface::Interface(QWidget *parent, SerialPort *serialPort) :
	ModuleWidget(parent, serialPort, true),
	m_outValue(NULL),
	m_slider(NULL)
{
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	QHBoxLayout *outValueLayout = new QHBoxLayout(this);
	mainLayout->addLayout(outValueLayout);

	QLabel *label = new QLabel("output value: ");
	outValueLayout->addWidget(label);
	m_outValue = new QLineEdit(this);
	m_outValue->setValidator( new QIntValidator(0, 1023, this) );
	connect(m_outValue, SIGNAL(returnPressed()), this, SLOT(SetValueFromLineEdit()));
	outValueLayout->addWidget(m_outValue);


	m_slider = new QSlider(Qt::Horizontal, this);
	m_slider->setMinimumSize(200, 20);
	m_slider->setRange(0, 1023);

	connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(SetValueFromSlider(int)));
	mainLayout->addWidget(m_slider);

	int value;
	if (m_serialPort->GetState(0, value))
	{
		m_slider->setValue(value);
		qDebug() << "value: " << value;
	}
	else
	{
		QMessageBox::critical(this, "", tr("Value was not set."));
	}

	unsigned mode;
	m_serialPort->GetMode(0, mode);


	setMaximumHeight(80);
}

Interface::~Interface()
{

}

void Interface::SetValueFromLineEdit()
{
	int iValue = atoi(m_outValue->text().toStdString().c_str());
	m_slider->setValue(iValue);
}
void Interface::SetValueFromSlider(int value)
{
	m_serialPort->SetValue(value);
	m_outValue->setText(QString("%1").arg(value));
}

