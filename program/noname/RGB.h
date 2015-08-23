#ifndef RGB_H
#define RGB_H

#include <ModuleWidget.h>
class SerialPort;
class RGB : public ModuleWidget
{
	Q_OBJECT
public:
	RGB(QWidget *parent, SerialPort *serialPort);
	~RGB();

signals:

public slots:
	void changeMode(int value);
};

#endif // RGB_H
