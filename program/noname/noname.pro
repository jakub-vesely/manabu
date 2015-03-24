#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T21:55:22
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = noname
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SerialPort.cpp \
    BootLoader.cpp

HEADERS  += MainWindow.h \
    SerialPort.h \
    BootLoader.h

FORMS    +=
