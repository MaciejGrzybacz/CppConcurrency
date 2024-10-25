cmake_minimum_required(VERSION 3.23)
project(PCP)

set(CMAKE_CXX_STANDARD 23)

add_executable(PCP main.cpp)

add_library(PCPLib INTERFACE include/ProducerConsumerProblem.hpp)
add_library(ChainLib INTERFACE include/ProcessChain.hpp)