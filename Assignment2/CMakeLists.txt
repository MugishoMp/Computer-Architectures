cmake_minimum_required(VERSION 3.0)
project(rv64-emu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(include)

add_subdirectory(src)
add_subdirectory(tst)
# add_subdirectory(lib/googletest)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

include_directories(lib/googletest/googletest)