#ifndef PLOT_H
#define PLOT_H

#include <QWidget>

class QCustomPlot;
class QTimer;

class Plot : public QWidget
{
	Q_OBJECT

	QCustomPlot *m_customPlot;

	QVector<double> m_x;
	QVector<double> m_y;

	QTimer *m_timer;
public:
	explicit Plot(QWidget *parent = 0);
	~Plot();

signals:

protected slots:
	void draw();
	void start();
	void stop();
};

#endif // PLOT_H
