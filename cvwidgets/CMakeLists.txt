 
cmake_minimum_required(VERSION 2.8)

project(cvwidgets)

find_package(Qt4 REQUIRED QtCore QtGui)
find_package(OpenCV REQUIRED)
include(${QT_USE_FILE})
include(GNUInstallDirs)

set(SOURCES
    cvimageviewer.cpp
    cvsimpleviewer.cpp
    genericimageviewer.cpp
    simpleimageviewer.cpp
    infodialog.cpp
    imagetablemodel.cpp
    cvmatprovider.cpp
)
set(QTHEADERS
    cvimageviewer.h
    cvsimpleviewer.h
    genericimageviewer.h
    simpleimageviewer.h
    infodialog.h
    imagetablemodel.h
)

set(HEADERS
    abstractimageprovider.h
    cvmatprovider.h
    cvwidgets_config.h
)

set(FORMS
    infodialog.ui
)

qt4_wrap_cpp(HEADERS_MOC ${QTHEADERS})
qt4_wrap_ui(FORMS_HEADERS ${FORMS})
qt4_add_resources(RESOURCES_RCC ${RESOURCES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(${QT_DEFINITIONS})
add_definitions(-DQDESIGNER_EXPORT_WIDGETS)
add_definitions(-DCVWIDGETS_LIBRARY)

add_library(${PROJECT_NAME} SHARED
    ${QTHEADERS}
    ${HEADERS}
    ${SOURCES}
    ${HEADERS_MOC}
    ${FORMS_HEADERS}
    ${RESOURCES_RCC}
)
target_link_libraries(${PROJECT_NAME}
    ${QT_LIBRARIES} ${OpenCV_LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADERS} ${QTHEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
