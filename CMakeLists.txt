cmake_minimum_required(VERSION 3.23)
project(Go)

set(CMAKE_CXX_STANDARD 14)

add_executable(Go main.cpp Game.cpp Game.h conio2.cpp conio2.h)
