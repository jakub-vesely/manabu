#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

class QWidget;
class Qstring;
class SerialPort;
class QAction;
class LogDialog;
class MainWindow : public QMainWindow
{
	Q_OBJECT

	void _ResetMainLayout();
	void _OpenBootloaderDialog(unsigned layer);

	SerialPort *m_serialPort;
	QWidget *m_mainWidget;
	QAction *m_flashInterface;
	QAction *m_flashNeighbor;
	QAction *m_logAction;
	LogDialog *m_log;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void openInterfaceBootloder();
	void openNeighborBootloader();
	void reintModules();
	void showLog(bool show);
	void uncheckLogAction(int notUsed);
};

#endif // MAINWINDOW_H
