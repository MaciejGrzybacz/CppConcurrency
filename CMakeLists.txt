cmake_minimum_required(VERSION 3.23)
project(CppConcurrency)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(BasicThreadOperations)
add_subdirectory(PCP)
add_subdirectory(FivePhilosophers)