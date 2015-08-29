#ifndef NOINTERFACE_H
#define NOINTERFACE_H

#include <ModuleWidget.h>

class SerialPort;

class NoInterface : public ModuleWidget
{
	Q_OBJECT
public:
	NoInterface(QWidget *parent, SerialPort *serialPort);
	~NoInterface();

signals:

public slots:
};

#endif // NOINTERFACE_H
