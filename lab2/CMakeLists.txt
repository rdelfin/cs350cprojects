cmake_minimum_required(VERSION 3.6)
project(lab2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c state.c array.c parser.c simulator.c)
add_executable(lab2 ${SOURCE_FILES})