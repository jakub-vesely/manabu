#ifndef NOINTERFACE_H
#define NOINTERFACE_H

#include <ModuleWidget.h>

class QLabel;
class SerialPort;
class QTimer;
class NoInterface : public ModuleWidget
{
	Q_OBJECT

    QLabel *m_label;
    QTimer *m_timer;
    unsigned m_tryCount;
public:
	NoInterface(QWidget *parent, SerialPort *serialPort);
	~NoInterface();

signals:
    void connected();
public slots:
    void tryConnect();
};

#endif // NOINTERFACE_H
