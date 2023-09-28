cmake_minimum_required(VERSION 3.16.3)

set (This Example)

project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(googletest)

set(Headers
    Example.hpp
)

set(Sources
    Example.cpp
)

add_library(${This} ${Sources} ${Headers})
# add subdirectory goes into that subdirectory and looks for a 
# cmake list there and runs that in a child node of the build
#system
add_subdirectory(test)