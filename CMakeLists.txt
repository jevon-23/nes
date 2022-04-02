# Skeleton from : https://github.com/ttroy50/cmake-examples/blob/master/01-basic/B-hello-headers/CMakeLists.txt
# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.2)

# Set the project name
project (nes)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/build/main.c
    )

# Add an executable with the above sources
add_executable(nes ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(nes
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    )

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Testing instruction set 
add_executable(test_instructions src/cpu/test/test.cpp src/cpu/cpu.c src/cpu/instruction_set.c src/mem/mem.c)

add_test(testAll test_instructions)

target_link_libraries(test_instructions gtest_main)

# Testing the utilites 
add_executable(test_utils src/utils/test/test.cpp src/utils/utils.c)

add_test(testAll, test_utils)

target_link_libraries(test_utils gtest_main)
