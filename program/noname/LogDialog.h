#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>
#include <ostream>
class QTextEdit;
class LogDialog : public QDialog
{
	Q_OBJECT

	QTextEdit *m_textEdit;
public:
	explicit LogDialog(QWidget *parent = 0);
	~LogDialog();
	void Info(const QString &string);
	void Warning(const QString &string);
	void Error(QString const &string);


signals:

public slots:
};

#endif // LOGDIALOG_H
