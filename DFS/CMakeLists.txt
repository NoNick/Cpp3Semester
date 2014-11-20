project(dfs)
cmake_minimum_required(VERSION 2.8)

find_package(Boost COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(SRC_LIST test.cpp graph.cpp graph.h gtest/gtest-all.cc gtest/gtest.h gtest/gtest_main.cc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -g -std=c++11 -Wall")

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME}  -lgmp -lgmpxx -lpthread ${Boost_LIBRARIES})
