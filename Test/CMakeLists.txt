cmake_minimum_required(VERSION 3.16.3)

project(Tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(Tests run_tests.cpp
        test_runner.h
        tests.h
        ../CSVTable.cpp
        ../utilities.cpp)
