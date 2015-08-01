#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <QWidget>
#include <vector>
#include <stdint.h>

class QLineEdit;
class QTextEdit;
class SerialPort;

class BootLoader : public QWidget
{
	Q_OBJECT

	QLineEdit *m_hexPath;
	QTextEdit *m_hexView;
	QTextEdit *m_status;
	QString m_directory;
	SerialPort *m_serialPort;
	std::vector<uint16_t> m_words;
	unsigned m_layer;

public:
	BootLoader(QWidget *parent, SerialPort *serialPort, bool layer);
	~BootLoader();

signals:

public slots:
	void openHex();
	void upload();
	void goToBootloader();

};

#endif // BOOTLOADER_H
