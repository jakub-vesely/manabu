#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <../../CommonConstants.h>

class QTabWidget;
class Qstring;
class SerialPort;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	void _SetMainLayout();
	char const *_GetModuleTypeName(ModuleTypes type);
	bool _AddInterfaceTab();
	void _AddRgbTab();
	void _AddBootloaderTab();
	void _AddPlotTab();

	SerialPort *m_serialPort;
	QTabWidget *m_tabWidget;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void changeLayer1Mode(int value);
};

#endif // MAINWINDOW_H
