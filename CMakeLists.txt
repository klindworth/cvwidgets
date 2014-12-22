cmake_minimum_required(VERSION 2.8)

option(BUILD_PLUGIN "Build QtDesigner Plugin" ON)

add_subdirectory(library)
if(BUILD_PLUGIN)
    add_subdirectory(designerplugin)
endif()

add_subdirectory(example)
