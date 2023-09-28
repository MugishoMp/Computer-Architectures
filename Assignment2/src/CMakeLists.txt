# ./src/CMakeLists.txt
set(BINARY ${CMAKE_PROJECT_NAME})

# Add your source files here
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cc *.cpp)

set(SOURCES ${SOURCES})

# Add an executable target
add_executable(rv64-emu ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES})

# Add the necessary flags
target_compile_options(rv64-emu PRIVATE
    -Wall
    -Weffc++
    -g
    -Og
)

# Check if the platform is macOS or WSL
if(APPLE)
    target_compile_options(rv64-emu PRIVATE 
        -stdlib=libc++
    )
    target_link_libraries(rv64-emu PRIVATE
        c++
        c++abi
    )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(rv64-emu PRIVATE 
        stdc++fs
    )
endif()


# Optionally, you can set properties or link against other libraries
# target_include_directories(my_library_name PRIVATE /path/to/other/include)
# target_link_libraries(my_library_name PRIVATE other_library)
