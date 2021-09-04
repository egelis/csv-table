cmake_minimum_required(VERSION 3.16.3)

project(TestTask)

set(CMAKE_CXX_STANDARD 20)

add_executable(TestTask main.cpp
        CSVTable.cpp
        CSVTable.h
        test_runner.h
        Tests.h
        utilities.h utilities.cpp)
