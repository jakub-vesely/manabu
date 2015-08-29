#include "ModuleWidget.h"
#include <SerialPort.h>
#include <QString>
ModuleWidget::ModuleWidget(QWidget *parent, SerialPort *serialPort, bool isInterface) :
	QGroupBox(parent),
	m_serialPort(serialPort)
{
	ModuleTypes moduleType = TYPE_NONE;
	if (NULL != m_serialPort && m_serialPort->IsOpen())
		m_serialPort->FillModuleType(isInterface ? 0 : 1, moduleType);

	setTitle(_GetModuleTypeName(moduleType));
}

ModuleWidget::~ModuleWidget()
{

}

QString ModuleWidget::_GetModuleTypeName(ModuleTypes type)
{
	switch(type)
	{
	case TYPE_NONE:
		return tr("Device not found");
	case TYPE_USB_INTERFACE:
		return tr("USB Interface");
	case TYPE_POTENTIOMETER:
		return tr("Potentiometer");
	case TYPE_RGB_LED:
		return tr("RGB LED");
	case TYPE_BUTTON:
		return tr("Button");
	case TYPE_ARITHMETIC_LOGIC:
		return tr("Arithmetic-logic");
	default:
		return tr("Unknown module");
	}
}

