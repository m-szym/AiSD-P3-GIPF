cmake_minimum_required(VERSION 3.23)
project(GIPF)

set(CMAKE_CXX_STANDARD 23)

add_executable(GIPF main.cpp Board.cpp Board.h Hex.cpp Hex.h constants.h GIPF.cpp GIPF.h other.cpp other.h AllTree.h Solver.cpp Solver.h)
