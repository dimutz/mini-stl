file(GLOB BENCH_SOURCES *.cpp)
add_executable(bench ${BENCH_SOURCES})
target_include_directories(bench PRIVATE ../include)
