cmake_minimum_required(VERSION 3.26)
project(chip8)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2 package
find_package(SDL2 REQUIRED)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest SDL)

#enable_testing()

add_executable(chip8 main.cpp
        Memory.cpp
        Memory.h
        Chip8.cpp
        Chip8.h
)

# Link executable with the SDL2 target
target_link_libraries(chip8
        PRIVATE SDL2::SDL2
#        GTest::gtest_main
)

#include(GoogleTest)
#gtest_discover_tests(chip8)