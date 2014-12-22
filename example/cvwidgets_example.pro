#-------------------------------------------------
#
# Project created by QtCreator 2014-01-02T13:06:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cvwidgets_example
TEMPLATE = app
LIBS += -lcvwidgets

SOURCES += main.cpp

HEADERS  +=

FORMS    += mainwindow.ui

unix {
	CONFIG += link_pkgconfig
	PKGCONFIG += opencv
}

