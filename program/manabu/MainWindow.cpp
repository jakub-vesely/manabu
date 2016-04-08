#include "MainWindow.h"
#include "SerialPort.h"
#include "BootLoader.h"

#include <NoInterface.h>
#include <QComboBox>
#include <QLineEdit>
#include <QSlider>
#include <QString>
#include <QMessageBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QThread>
#include <Plot.h>
#include <Interface.h>
#include <RGB.h>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <LogDialog.h>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_serialPort(NULL),
	m_mainWidget(NULL),
	m_flashInterface(NULL),
	m_flashNeighbor(NULL),
	m_logAction(NULL),
	m_log(NULL)
{
	setMinimumSize(320, 480);

	m_log = new  LogDialog(this);
	connect(m_log, SIGNAL(finished(int)), this, SLOT(uncheckLogAction(int)));

	QMenu *menu = this->menuBar()->addMenu(tr("menu"));
	QAction *refresh = menu->addAction(tr("&Refresh"));
	refresh->setShortcuts(QKeySequence::Refresh);
	connect(refresh, SIGNAL(triggered()), this, SLOT(reintModules()));

	m_logAction = menu->addAction(tr("&Log"));
	m_logAction->setCheckable(true);
	m_logAction->setChecked(false);
	connect(m_logAction, SIGNAL(triggered(bool)), this, SLOT(showLog(bool)));

	menu->addSeparator();
	m_flashInterface = menu->addAction(tr("&Interface to bootloader"));
	connect(m_flashInterface, SIGNAL(triggered()), this, SLOT(openInterfaceBootloder()));

	m_flashNeighbor = menu->addAction(tr("&Flash neighbor"));
	connect(m_flashNeighbor, SIGNAL(triggered()), this, SLOT(openNeighborBootloader()));

	menu->addSeparator();
	QAction *exit = menu->addAction(tr("E&xit"));
	exit->setShortcuts(QKeySequence::Close);
	connect(exit, SIGNAL(triggered()), this, SLOT(close()));

	reintModules();
}

void MainWindow::uncheckLogAction(int notUsed)
{
	m_logAction->setChecked(false);

}
void MainWindow::showLog(bool show)
{
	if (show)
		m_log->show();
	else
		m_log->hide();

}

void MainWindow::reintModules()
{
	_ResetMainLayout();

	if (NULL == m_serialPort)
		m_serialPort = new SerialPort(this, m_log);

	if (m_serialPort->IsOpen())
		m_serialPort->Close();

	if (!m_serialPort->Open())
	{
        NoInterface *noInterface =new NoInterface(m_mainWidget, m_serialPort);
        connect(noInterface, SIGNAL(connected()), this, SLOT(reintModules()));
        m_mainWidget->layout()->addWidget(noInterface);
		m_flashInterface->setEnabled(false);
	}
	else
	{
		m_mainWidget->layout()->addWidget(new Interface(m_mainWidget, m_serialPort));
		m_flashInterface->setEnabled(true);

		unsigned version;
		bool layer1Connected = true;
		try
		{
			version = m_serialPort->GetFlashVersion(1);

		}
		catch (...)
		{
			layer1Connected = false;
		}

		m_flashNeighbor->setEnabled(layer1Connected);
		if (layer1Connected)
		{
			if (0 == version)
			{
				ModuleTypes moduleType;

				if(!(ModuleTypes)m_serialPort->FillModuleType(1, moduleType))
					return; //no module connected to the interface

				if (moduleType == TYPE_RGB_LED)
					m_mainWidget->layout()->addWidget(new RGB(m_mainWidget, m_serialPort));
				else
					m_mainWidget->layout()->addWidget(new Plot(m_mainWidget, m_serialPort));
			}
			else
				m_log->Info("module with a bootloader connected");

		}
		else
		{
			((QVBoxLayout*)m_mainWidget->layout())->addStretch();
		}
	}
}
void MainWindow::_ResetMainLayout()
{
	if (NULL != m_mainWidget)
		delete m_mainWidget;

	m_mainWidget = new QWidget(this);
	this->setCentralWidget(m_mainWidget);
	new QVBoxLayout(m_mainWidget);

}


void MainWindow::_OpenBootloaderDialog(unsigned layer)
{

	BootLoader *bootLoader = new BootLoader(this, m_serialPort, layer, m_log);
	connect(bootLoader, SIGNAL(flashed()), this, SLOT(reintModules()));
	bootLoader->exec();
}

void MainWindow::openInterfaceBootloder()
{
	_OpenBootloaderDialog(0);
}
void MainWindow::openNeighborBootloader()
{
	_OpenBootloaderDialog(1);
}

MainWindow::~MainWindow()
{
}
