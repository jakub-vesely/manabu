#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <QWidget>

class QLineEdit;
class QTextEdit;

class BootLoader : public QWidget
{
	Q_OBJECT

	QLineEdit *m_hexPath;
	QTextEdit *m_textEdit;
	QString m_directory;
public:
	explicit BootLoader(QWidget *parent = 0);
	~BootLoader();

signals:

public slots:
	void openHex();
	void upload();

};

#endif // BOOTLOADER_H
