cmake_minimum_required(VERSION 3.28.1)
project(FoataNormalForm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_MAKE_PROGRAM "mingw32-make")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(FNF
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/src/InputParser.cpp
        ${PROJECT_SOURCE_DIR}/src/Solver.cpp
)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()