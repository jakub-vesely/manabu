#ifndef PLOT_H
#define PLOT_H

#include <QWidget>

class QCustomPlot;
class QTimer;
class SerialPort;

class Plot : public QWidget
{
	Q_OBJECT

	QCustomPlot *m_customPlot;

	QVector<double> m_x;
	QVector<double> m_y;

	QTimer *m_timer;
	SerialPort *m_serialPort;

public:
	explicit Plot(QWidget *parent, SerialPort *serialPort);
	~Plot();

signals:

protected slots:
	void draw();
	void start();
	void stop();
};

#endif // PLOT_H
