cmake_minimum_required(VERSION 3.23)
project(BasicThreadsOperations)

set(CMAKE_CXX_STANDARD 23)

add_executable(BTO test_1.cpp)
add_library(Counter INTERFACE Counter.hpp)