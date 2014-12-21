 
cmake_minimum_required(VERSION 2.8)

project(cvwidgets_example)

find_package(Qt5Widgets)
find_package(OpenCV REQUIRED)
#include(${QT_USE_FILE})

set(SOURCES
    main.cpp
)

add_executable(${PROJECT_NAME} 
    ${SOURCES}
)
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets ${OpenCV_LIBS} cvwidgets
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
 
