cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 17)

project(sudoku)

add_executable(sudoku fast_solve.cpp)