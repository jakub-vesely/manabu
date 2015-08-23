#include "RGB.h"
#include <SerialPort.h>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
RGB::RGB(QWidget *parent, SerialPort *serialPort) :
	ModuleWidget(parent, serialPort, false)
{
	QVBoxLayout *vLayout = new QVBoxLayout(this);
	QHBoxLayout *hLayout = new QHBoxLayout(this);
	vLayout->addLayout(hLayout);

	QLabel *label = new QLabel(tr("mode: "), this);
	hLayout->addWidget(label);
	QComboBox *modeCombo = new QComboBox(this);
	modeCombo->addItem(tr("red to red"));
	modeCombo->addItem(tr("red to purple"));
	modeCombo->addItem(tr("white value"));
	hLayout->addWidget(modeCombo);

	hLayout->addStretch();

	unsigned mode;
	m_serialPort->GetMode(1, mode);
	modeCombo->setCurrentIndex(mode-1);

	connect(modeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMode(int)));

	vLayout->addStretch();
}

RGB::~RGB()
{

}

void RGB::changeMode(int value)
{
	m_serialPort->SetMode(1, value+1);
}

