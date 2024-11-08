cmake_minimum_required(VERSION 3.14)
project(List)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(ListTests
        PerformanceTest.cpp
)

target_link_libraries(ListTests
        PRIVATE
        GTest::gtest_main
        GTest::gmock_main
        pthread
)

include(GoogleTest)
gtest_discover_tests(ListTests)

add_executable(List main.cpp)