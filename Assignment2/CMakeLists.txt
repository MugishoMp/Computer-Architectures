# ./CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(rv64-emu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(GTEST_SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/googletest)

include_directories(include)
include_directories(${GTEST_SOURCE_DIR}/include)

add_subdirectory(${GTEST_SOURCE_DIR} ${CMAKE_BINARY_DIR}/gtest)
# Add the source directory
add_subdirectory(src)
# Add the test directory
add_subdirectory(tst)