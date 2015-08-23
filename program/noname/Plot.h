#ifndef PLOT_H
#define PLOT_H

#include <ModuleWidget.h>

class QCustomPlot;
class QTimer;
class QLineEdit;
class SerialPort;
class Plot : public ModuleWidget
{
	Q_OBJECT

	QCustomPlot *m_customPlot;

	QVector<double> m_x;
	QVector<double> m_y;

	QTimer *m_timer;
	SerialPort *m_serialPort;
	QLineEdit *m_outValue;

public:
	Plot(QWidget *parent, SerialPort *serialPort);
	~Plot();

signals:

protected slots:
	void draw();
	void clear();
};

#endif // PLOT_H
