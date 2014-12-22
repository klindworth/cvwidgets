 
cmake_minimum_required(VERSION 3.0)

project(cvwidgets)

option(BUILD_UNITTESTS "Builds the gtest unittests" OFF)

find_package(Qt5Widgets)
find_package(OpenCV REQUIRED)
include(GNUInstallDirs)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(${QT_DEFINITIONS})
add_definitions(-DQDESIGNER_EXPORT_WIDGETS)
add_definitions(-DCVWIDGETS_LIBRARY)

add_library(${PROJECT_NAME} SHARED
    ${HEADERS}
    ${SOURCES}
    ${FORMS_HEADERS}
)
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets ${OpenCV_LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADERS} ${QTHEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")



find_package(GTest)
if(GTEST_FOUND)
    enable_testing()
    set(TESTSOURCES
        cvmatprovider.cpp
        cvmatprovider_tests.cpp
    )
    set(TESTHEADERS
        cvmatprovider.h
    )
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME}_tests ${TESTHEADERS} ${TESTSOURCES})
    add_test(NAME test COMMAND ${PROJECT_NAME}_tests)
    target_link_libraries(${PROJECT_NAME}_tests Qt5::Widgets ${OpenCV_LIBS} ${GTEST_BOTH_LIBRARIES})
endif()
