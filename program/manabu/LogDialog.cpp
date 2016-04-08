#include "LogDialog.h"
#include <QTextEdit>
#include <QPalette>
LogDialog::LogDialog(QWidget *parent) :
	QDialog(parent),
	m_textEdit(NULL)
{
	m_textEdit = new QTextEdit(this);
	m_textEdit->setStyleSheet("QTextEdit { background-color : black;}");
	m_textEdit->setMinimumSize(320,480);
}

LogDialog::~LogDialog()
{

}

void LogDialog::Info(const QString &string)
{
	m_textEdit->setTextColor(QColor(Qt::white));
	m_textEdit->append(string);

}

void LogDialog::Warning(const QString &string)
{
	m_textEdit->setTextColor(QColor(Qt::yellow));
	m_textEdit->append(string);

}

void LogDialog::Error(const QString &string)
{
	m_textEdit->setTextColor(QColor(Qt::red));
	m_textEdit->append(string);

}

