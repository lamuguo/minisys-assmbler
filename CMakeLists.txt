cmake_minimum_required(VERSION 3.5)
project(myassembler)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp assembler.cpp callAss.cpp)
add_executable(myassembler ${SOURCE_FILES})