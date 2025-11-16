file(GLOB TEST_SOURCES *.cpp)
add_executable(tests ${TEST_SOURCES})
target_include_directories(tests PRIVATE ../include)
