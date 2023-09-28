# Assuming you have GTest available
# find_package(GTest REQUIRED)

# Add test files
add_executable(getA_test getA_test.cpp)

# Link against GTest, the main project library, and any other necessary libraries
target_link_libraries(getA_test gtest gtest_main rv64-emu_lib)

target_link_libraries(getA_test gtest_main)
# Register the test
add_test(NAME GetATest COMMAND getA_test)
