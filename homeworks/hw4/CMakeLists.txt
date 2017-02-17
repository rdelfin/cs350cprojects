cmake_minimum_required(VERSION 3.6)
project(hw4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES main.c bitmatrix.c bitops.c)
add_executable(hw4 ${SOURCE_FILES})
