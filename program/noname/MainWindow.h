#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTabWidget;
class Qstring;
class SerialPort;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	void _SetMainLayout();
	bool _AddInterfaceTab();
	void _AddRgbTab();
	void _AddBootloaderTab();

	SerialPort *m_serialPort;
	QTabWidget *m_tabWidget;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void modeChanged(int value);
};

#endif // MAINWINDOW_H
