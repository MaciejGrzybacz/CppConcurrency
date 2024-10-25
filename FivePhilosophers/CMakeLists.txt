cmake_minimum_required(VERSION 3.23)
project(BasicThreadsOperations)

set(CMAKE_CXX_STANDARD 23)

add_library(philosopher_lib
        src/Philosopher.cpp
        src/PhilosopherImpl.cpp
        src/PhilosopherFactory.cpp
        include/PhilosopherFactory.hpp
        include/Dinner.hpp
        src/DInner.cpp
)

target_include_directories(philosopher_lib PUBLIC include)

add_executable(main_exec main.cpp)

target_link_libraries(main_exec PRIVATE philosopher_lib)
