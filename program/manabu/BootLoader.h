#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <QDialog>
#include <vector>
#include <stdint.h>

class QLineEdit;
class QTextEdit;
class SerialPort;
class QPushButton;
class LogDialog;

class BootLoader : public QDialog
{
	Q_OBJECT

	QLineEdit *m_hexPath;
	QTextEdit *m_hexView;
	QTextEdit *m_status;
	QString m_directory;
	SerialPort *m_serialPort;
	std::vector<uint16_t> m_words;
	unsigned m_layer;
	QPushButton *m_uploadButton;
	LogDialog *m_log;
public:
	BootLoader(QWidget *parent, SerialPort *serialPort, bool layer, LogDialog *logDialog);
	~BootLoader();

signals:
    void flashed();
public slots:
	void openHex();
	void upload();
	void goToBootloader();

};

#endif // BOOTLOADER_H
