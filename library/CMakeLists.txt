 
cmake_minimum_required(VERSION 3.0)

project(cvio)

find_package(Qt5Widgets)
find_package(OpenCV REQUIRED core imgproc highgui)
find_package(Boost REQUIRED filesystem system)
find_package(OpenMP)
option(BUILD_HDF5_SUPPORT "Build HDF5 Support" ON)

if(BUILD_HDF5_SUPPORT)
    find_package(HDF5 REQUIRED)
endif()


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

if(BUILD_HDF5_SUPPORT)
    set(SOURCES
        ${SOURCES}
        hdf5internals.cpp
        hdf5wrapper.cpp
        h5types.cpp
    )

    set(HEADERS
        ${HEADERS}
        hdf5internals.h
        hdf5wrapper.h
        h5types.h
    )
endif()

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
    Qt5::Widgets ${OpenCV_LIBS} ${HDF5_LIBRARIES} ${Boost_LIBRARIES}
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
    if(BUILD_HDF5_SUPPORT)
        set(TESTSOURCES ${TESTSOURCES} tests.cpp hdf5internals.cpp hdf5wrapper.cpp h5types.cpp)
    endif()
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME}_tests ${TESTHEADERS} ${TESTSOURCES})
    add_test(NAME test COMMAND ${PROJECT_NAME}_tests)
    target_link_libraries(${PROJECT_NAME}_tests Qt5::Widgets ${OpenCV_LIBS} ${GTEST_BOTH_LIBRARIES} ${HDF5_LIBRARIES} ${Boost_LIBRARIES})
endif()
