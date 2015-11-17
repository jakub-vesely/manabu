#ifndef BUTTON_H
#define BUTTON_H

#include <ModuleWidget.h>
class QWidget;
class SerialPort;
class Button : public ModuleWidget
{
    Q_OBJECT
public:
    Button(QWidget *parent, SerialPort *serialPort);
public slots:
    void changeMode(int value);
};

#endif // BUTTON_H
