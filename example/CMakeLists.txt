 
cmake_minimum_required(VERSION 2.8)

project(cvio_example)

find_package(Qt5Widgets)
find_package(OpenCV REQUIRED)

set(SOURCES
    main.cpp
)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} 
    ${SOURCES}
)
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets ${OpenCV_LIBS} cvio
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
 
