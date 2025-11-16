cmake_minimum_required(VERSION 3.16)
project(mini_stl)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)

add_executable(mini_stl main.cpp)

add_executable(tests
    test/main.cpp
    test/test_vector.cpp
    test/test_list.cpp
    test/test_map.cpp
)
target_include_directories(tests PRIVATE include)

add_executable(bench
    benchmark/main.cpp
    benchmark/bench_vector.cpp
    benchmark/bench_map.cpp
)
target_include_directories(bench PRIVATE include)
