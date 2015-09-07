#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H

#include <../../CommonConstants.h>
#include <QGroupBox>

class SerialPort;
class QString;

class ModuleWidget : public QGroupBox
{
	Q_OBJECT
protected:
	QString _GetModuleTypeName(ModuleTypes type);

    SerialPort *m_serialPort;
public:
	ModuleWidget(QWidget *parent, SerialPort *serialPort, bool isInterface);
	~ModuleWidget();

signals:

public slots:
};

#endif // MODULEWIDGET_H
