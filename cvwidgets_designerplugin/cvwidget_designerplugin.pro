CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(cvimageviewerplugin)
TEMPLATE    = lib

HEADERS     = cvimageviewerplugin.h \
    cvsimpleviewerplugin.h \
    cvwidgetsset.h
SOURCES     = cvimageviewerplugin.cpp \
    cvsimpleviewerplugin.cpp \
    cvwidgetsset.cpp
RESOURCES   =
LIBS        += -L. -lcvwidgets

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

FORMS +=
