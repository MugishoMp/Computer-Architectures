cmake_minimum_required(VERSION 3.16.3)

set(This ExampleTests)

set(Sources
    ExampleTests.cpp
)

# this is similar to the add library but instead of 
# making a component that  is linked into a program
# we are actually making the program itself
add_executable(${This} ${Sources}

)
# things that you depend on you would use tll
# PUBLIC it is not just you but anything using the program 
# would depend on it as well
# gtest_main: is provided by googletest which pulls in the main
# program. So google test provides the main program for you, 
# and you're linking it in with your tests
# example: and then we need the thing we are testing of course
target_link_libraries( ${This} PUBLIC
    gtest_main
    Example
)

add_test(
    NAME ${This}
    COMMAND ${This}
)
