cmake_minimum_required(VERSION 3.23)
project(PCP)

set(CMAKE_CXX_STANDARD 23)

add_executable(PCP test_3.cpp)

add_library(PCPLib INTERFACE ProducerConsumerProblem.hpp)
add_library(ChainLib INTERFACE ProcessChain.hpp)