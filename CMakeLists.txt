cmake_minimum_required(VERSION 3.18)

# set the project name
project(GPR5204)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(units CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
    
file(GLOB_RECURSE SRC_FILES include/*.h src/*.cpp)
add_library(Common STATIC ${SRC_FILES})
target_include_directories(Common PUBLIC "include/")
target_link_libraries(Common PUBLIC units::units)

file(GLOB_RECURSE TEST_FILES test/*.cpp)
add_executable(CommonTest ${TEST_FILES})
target_link_libraries(CommonTest PRIVATE Common)
target_link_libraries(CommonTest PRIVATE GTest::gtest GTest::gtest_main)


file(GLOB_RECURSE BENCH_FILES bench/*.cpp)
add_executable(CommonBench ${BENCH_FILES})
target_link_libraries(CommonBench PRIVATE Common)
target_link_libraries(CommonBench PRIVATE benchmark::benchmark benchmark::benchmark_main)