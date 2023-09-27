
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# # Now simply link against gtest or gtest_main as needed. Eg
add_executable(getA_test getA_test.cpp)
target_link_libraries(getA_test gtest_main)
add_test(NAME getA_test COMMAND getA_test)
