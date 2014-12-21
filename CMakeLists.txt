cmake_minimum_required(VERSION 2.8)

option(BUILD_PLUGIN "Build QtDesigner Plugin" ON)

add_subdirectory(cvwidgets)
if(BUILD_PLUGIN)
    add_subdirectory(cvwidgets_designerplugin)
endif()

add_subdirectory(cvwidgets_example)
