 
cmake_minimum_required(VERSION 2.8)

set(PROJECT cvwidgets)
project(${PROJECT})

# Include the other CMakeLists.txt
find_package(Qt4 REQUIRED QtCore QtGui)
find_package(OpenCV REQUIRED)
include(${QT_USE_FILE})

# Files to insert
set(SOURCES
    cvimageviewer.cpp
    cvsimpleviewer.cpp
    genericimageviewer.cpp
    simpleimageviewer.cpp
    infodialog.cpp
    imagetablemodel.cpp
    cvmatprovider.cpp
)
set(HEADERS
    cvimageviewer.h
    cvsimpleviewer.h
    genericimageviewer.h
    simpleimageviewer.h
    infodialog.h
    imagetablemodel.h
    abstractimageprovider.h
    cvmatprovider.h
    cvwidgets_config.h
)
set(FORMS
    infodialog.ui
)

qt4_wrap_cpp(HEADERS_MOC ${HEADERS})
qt4_wrap_ui(FORMS_HEADERS ${FORMS})
qt4_add_resources(RESOURCES_RCC ${RESOURCES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(${QT_DEFINITIONS})
# I'm no expert in libraries so, intuitively I'd say this is useless but it won't compile if I don't define it.
# This clearly needs to get fixed.
add_definitions(-DQDESIGNER_EXPORT_WIDGETS)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS_MOC}
    ${FORMS_HEADERS}
    ${RESOURCES_RCC}
)
target_link_libraries(${PROJECT}
    # Uncomment the following if you want to target Plugins
    #Plugins
    ${QT_LIBRARIES} ${OpenCV_LIBS}
)

install(TARGETS ${PROJECT} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})