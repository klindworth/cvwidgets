 
cmake_minimum_required(VERSION 2.8)

set(PROJECT cvio_designerplugin)
project(${PROJECT})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Designer REQUIRED)
find_package(OpenCV REQUIRED)
set(QT_USE_QTDESIGNER TRUE)
set(CMAKE_AUTOMOC ON)

set(SOURCES
    cvimageviewerplugin.cpp
    cvsimpleviewerplugin.cpp
    simplehistogramplugin.cpp
    cvwidgetsset.cpp
)

set(HEADERS
    cvimageviewerplugin.h
    cvsimpleviewerplugin.h
    simplehistogramplugin.h
    cvwidgetsset.h
)


#I'm not sure every define is necessary
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)
add_definitions(-DQDESIGNER_EXPORT_WIDGETS)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS_MOC}
)
target_link_libraries(${PROJECT} Qt5::Widgets Qt5::Designer ${OpenCV_LIBS} cvio)

set(PLUGIN_DIR ${_qt5Designer_install_prefix}/plugins/designer)

# Install the library in QtDesigner plugin directory
install(TARGETS ${PROJECT}
    DESTINATION ${PLUGIN_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
