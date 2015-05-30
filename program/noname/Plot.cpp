#include "Plot.h"
#include "../extern/qcustomplot/qcustomplot.h"
#include <QTimer>
#include <math.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "SerialPort.h"

Plot::Plot(QWidget *parent, SerialPort *serialPort) :
	QWidget(parent),
	m_customPlot(NULL),
	m_serialPort(serialPort)
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	m_customPlot = new QCustomPlot(this);
	layout->addWidget(m_customPlot);

	m_customPlot->addGraph();
	m_customPlot->xAxis->setLabel("x");
	m_customPlot->yAxis->setLabel("y");
	m_customPlot->xAxis->setRange(0, 1);
	m_customPlot->yAxis->setRange(0, 255);

	QHBoxLayout *buttons = new QHBoxLayout(this);
	layout->addLayout(buttons);

	QPushButton *m_startButton = new QPushButton(tr("Start"), this);
	buttons->addWidget(m_startButton);
	connect(m_startButton, SIGNAL(clicked()), this, SLOT(start()));

	QPushButton *m_stopButton = new QPushButton(tr("Stop"), this);
	buttons->addWidget(m_stopButton);
	connect(m_stopButton, SIGNAL(clicked()), this, SLOT(stop()));

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(draw()));
}

void Plot::draw()
{
	unsigned size = m_x.size();
	m_x.resize(size + 1);
	m_y.resize(size + 1);

	m_x[size] = size; // x goes from -1 to 1
	m_y[size] = m_serialPort->GetValue(1);

	m_customPlot->xAxis->setRange(0, m_x[size]);
	//m_customPlot->yAxis->setRange(0, m_y[size]);
	m_customPlot->graph(0)->setData(m_x, m_y);
	m_customPlot->replot(QCustomPlot::rpImmediate);
}

void Plot::start()
{
	m_x.clear();
	m_y.clear();
	m_timer->start(200);
}

void Plot::stop()
{
	m_timer->stop();
}


Plot::~Plot()
{

}

