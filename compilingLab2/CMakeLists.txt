cmake_minimum_required(VERSION 3.8)
project(compilingLab2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp symbolTable.cpp symbolTable.h)
add_executable(compilingLab2 ${SOURCE_FILES})