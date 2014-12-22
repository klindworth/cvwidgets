#-------------------------------------------------
#
# Project created by QtCreator 2013-12-22T14:20:38
#
#-------------------------------------------------

QT       += widgets

TARGET = cvwidgets
TEMPLATE = lib

DEFINES += CVWIDGETS_LIBRARY

SOURCES += \
    cvimageviewer.cpp \
    cvsimpleviewer.cpp \
    genericimageviewer.cpp \
    simpleimageviewer.cpp \
    infodialog.cpp \
    imagetablemodel.cpp \
    cvmatprovider.cpp

HEADERS += \
    cvimageviewer.h \
    cvsimpleviewer.h \
    genericimageviewer.h \
    simpleimageviewer.h \
    infodialog.h \
    imagetablemodel.h \
    abstractimageprovider.h \
    cvmatprovider.h \
    cvwidgets_config.h

headers.path    = /usr/include
headers.files   += $$HEADERS
INSTALLS       += headers

unix {
    target.path = /usr/lib64
    INSTALLS += target
}

FORMS += \
    infodialog.ui
