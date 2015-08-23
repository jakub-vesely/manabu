#ifndef ITERFACE_H
#define ITERFACE_H

#include <ModuleWidget.h>

class SerialPort;
class QSlider;
class QLineEdit;
class QString;

class Interface : public ModuleWidget
{
	Q_OBJECT
	QLineEdit *m_outValue;
	QSlider *m_slider;
public:
	Interface(QWidget *parent, SerialPort *serialPort);
	~Interface();
signals:

public slots:
	void SetValueFromLineEdit();
	void SetValueFromSlider(int value);
};

#endif // ITERFACE_H
