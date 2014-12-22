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
Requirements: OpenCV2, Qt5 (there's a Qt4 branch available)

* Switch to the project directory of cvwidgets
* cmake . && make
* su root
* (type in your superuser password)
* make install

## Usage
A project example is given in the subfolder example

### via qmake
If you use qmake (the default build system of QtCreator), then add this line to your .pro file:

`LIBS += -lcvwidgets`

(like in the example/cvwidgets_example.pro file)

### via cmake
Add to your CMakeLists.txt

`target_link_libraries(your_project_name cvwidgets)`

(like in the example/CMakeLists.txt)
