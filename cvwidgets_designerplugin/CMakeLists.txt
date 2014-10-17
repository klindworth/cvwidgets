 
cmake_minimum_required(VERSION 2.8)

set(PROJECT cvimageviewerplugin)
project(${PROJECT})

find_package(Qt4 REQUIRED QtCore QtGui)
find_package(OpenCV REQUIRED)
set(QT_USE_QTDESIGNER TRUE)
include(${QT_USE_FILE})

# Needed to compile against ui and moc generated files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    cvimageviewerplugin.cpp
    cvsimpleviewerplugin.cpp
    cvwidgetsset.cpp
)

set(HEADERS
    cvimageviewerplugin.h
    cvsimpleviewerplugin.h
    cvwidgetsset.h
)


qt4_wrap_cpp(HEADERS_MOC ${HEADERS})

# Here too, I'm not sure every define is necessary
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)
add_definitions(-DQDESIGNER_EXPORT_WIDGETS)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS_MOC}
)
target_link_libraries(${PROJECT} ${QT_LIBRARIES} ${OpenCV_LIBS} cvwidgets)

# Install the library in QtDesigner plugin directory
install(TARGETS ${PROJECT}
    DESTINATION ${QT_PLUGINS_DIR}/designer
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
