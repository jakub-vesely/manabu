#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T21:55:22
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = manabu
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SerialPort.cpp \
    BootLoader.cpp \
    ../extern/qcustomplot/qcustomplot.cpp \
    Plot.cpp \
    ModuleWidget.cpp \
    Interface.cpp \
    RGB.cpp \
    NoInterface.cpp \
    LogDialog.cpp \
    Button.cpp

HEADERS  += MainWindow.h \
    SerialPort.h \
    BootLoader.h \
    ../extern/qcustomplot/qcustomplot.h \
    Plot.h \
    ../../CommonConstants.h \
    ModuleWidget.h \
    Interface.h \
    RGB.h \
    NoInterface.h \
    LogDialog.h \
    Button.h

FORMS    +=
