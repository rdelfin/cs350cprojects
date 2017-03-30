cmake_minimum_required(VERSION 3.0)
project(hw6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

set(SOURCE_FILES
        src/args.cpp
        src/assembler.cpp
        src/instruction.cpp
        src/tree.cpp
        src/parser.cpp
        )

add_executable(hw6 ${SOURCE_FILES})
