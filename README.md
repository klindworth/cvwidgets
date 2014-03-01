CVWidgets Library
=================

## Short description
The library provides a Qt widget for viewing OpenCV matrices. It can show various formats like CV_8UC, CV_16US, CV_32F,...

Some other features:

* Zoom
* Simple markers
* Saving the graphical representation (but without markers)
* Table for viewing the pixel values
* Qt Designer plugin available

## Install
Requirements: OpenCV2, Qt4 (Qt 5 not tested)

### via qmake

Note: If the library should be installed to another directory than /usr/lib64, you have to modify the cvwidget.pro file.

* Switch to the project directory of cvwidgets
* qmake && make
* su root
* (type in your superuser password)
* make install

Do the same in the cvwidgets_designerplugin directory if you want to install the Qt Designer plugin.

### via cmake

* Switch to the project directory of cvwidgets
* cmake . && make
* su root
* (type in your superuser password)
* make install

## Usage
If you use qmake (the default build system of QtCreator), then add this line to your .pro file:

LIBS += -lcvwidgets