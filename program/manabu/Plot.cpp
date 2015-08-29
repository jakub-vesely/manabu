#include "Plot.h"
#include "../extern/qcustomplot/qcustomplot.h"
#include <QTimer>
#include <math.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "SerialPort.h"
#include <QLineEdit>
Plot::Plot(QWidget *parent, SerialPort *serialPort) :
	ModuleWidget(parent, serialPort, false),
	m_customPlot(NULL),
	m_serialPort(serialPort),
	m_outValue(NULL)
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	QHBoxLayout *outValueLayout = new QHBoxLayout(this);
	layout->addLayout(outValueLayout);
	QLabel *label = new QLabel("output value: ");
	outValueLayout->addWidget(label);
	m_outValue = new QLineEdit(this);
	m_outValue->setReadOnly(true);
	outValueLayout->addWidget(m_outValue);

	m_customPlot = new QCustomPlot(this);
	layout->addWidget(m_customPlot);

	m_customPlot->addGraph();
	m_customPlot->xAxis->setLabel("x");
	m_customPlot->yAxis->setLabel("y");
	m_customPlot->xAxis->setRange(0, 1);
	m_customPlot->yAxis->setRange(0, 1024);

	QPushButton *m_stopButton = new QPushButton(tr("Clear"), this);
	layout->addWidget(m_stopButton);
	connect(m_stopButton, SIGNAL(clicked()), this, SLOT(clear()));

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(draw()));
	m_timer->start(1000);
}

void Plot::draw()
{
	unsigned size = m_x.size();
	m_x.resize(size + 1);
	m_y.resize(size + 1);

	m_x[size] = size; // x goes from -1 to 1

	int state;
	if (m_serialPort->GetState(1, state))
	{
		m_y[size] = state;
		m_outValue->setText(QString("%1").arg(state));
	}
	else
	{
		m_y[size] = 0;
		m_outValue->setText(tr("N/A"));
	}

	m_customPlot->xAxis->setRange(0, m_x[size]);
	//m_customPlot->yAxis->setRange(0, m_y[size]);
	m_customPlot->graph(0)->setData(m_x, m_y);
	m_customPlot->replot(QCustomPlot::rpImmediate);
}

void Plot::clear()
{
	m_x.clear();
	m_y.clear();
}


Plot::~Plot()
{

}

