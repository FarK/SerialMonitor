#-------------------------------------------------
#
# Project created by QtCreator 2012-03-04T23:24:55
#
#-------------------------------------------------

QT       += core gui

TARGET = SerialMonitor
TEMPLATE = app

INCLUDEPATH += /usr/include/boost

LIBS += -lboost_system \
	-lboost_thread-mt \
	-lboost_date_time

SOURCES += main.cpp\
	serialMonitor.cpp \
	plotter.cpp \
	plotterCore.cpp \
	qcustomplot.cpp \
	serial.cpp \
	checkableQAction.cpp \
	serialPortsQMenu.cpp

HEADERS  += plotter.h \
	serialMonitor.h \
	plotterCore.h \
	qcustomplot.h \
	serial.h \
	checkableQAction.h \
	serialPortsQMenu.h

FORMS    += plotterDesign.ui \
	serialMonitor.ui


