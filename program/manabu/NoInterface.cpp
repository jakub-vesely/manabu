#include "NoInterface.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include <SerialPort.h>

#define TRY_MAX  30

NoInterface::NoInterface(QWidget *parent, SerialPort *serialPort) :
    ModuleWidget(parent, serialPort, true),
    m_tryCount(0)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    m_label = new QLabel(this);
    m_label->setWordWrap(true);
    if (m_serialPort->FoundButClosed())
    {
        m_label->setText(QString(tr("Try to connect to a device on a port %1...").arg(m_serialPort->GetPortName())));


        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(tryConnect()));
        m_timer->start(1000);
    }
    else
    {
        m_label->setText(tr("No interface is connected."));
    }
    layout->addWidget(m_label);
}

NoInterface::~NoInterface()
{

}

void NoInterface::tryConnect()
{
    if (m_serialPort->Open())
    {
        connected();
        return;
    }

    if (!m_serialPort->FoundButClosed())
    {
        m_label->setText(QString(tr("Interface has been disconnected.")));
        m_timer->stop();
        return;
    }

    if (++m_tryCount == TRY_MAX)
    {
        m_timer->stop();
        m_label->setText(QString(tr("Device on the port %1 is unaccessible.")).arg(m_serialPort->GetPortName()));
    }
}

