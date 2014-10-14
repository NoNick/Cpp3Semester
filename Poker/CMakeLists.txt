cmake_minimum_required(VERSION 2.8.4)
project(Poker)

set(SOURCE_FILES_TEST poker_testing.cpp
                 texas_holdem.h
                 texas_holdem.cpp
                 game_factory.cpp
                 cards.h
                 cards.cpp
                 player.h
                 human_player.h
                 human_player.cpp
                 board_state.h
                 board_state.cpp
                 textui.h
                 textui.cpp
                 gtest/gtest-all.cc
                 gtest/gtest.h
                 gtest/gtest_main.cc)
set(SOURCE_FILES texas_holdem.h
                 texas_holdem.cpp
                 game_factory.cpp
                 cards.h
                 cards.cpp
                 player.h
                 human_player.h
                 human_player.cpp
                 board_state.h
                 board_state.cpp
                 textui.h
                 textui.cpp
                 main.cpp)
add_executable(Poker_test ${SOURCE_FILES_TEST})
add_executable(Poker ${SOURCE_FILES})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -g -std=c++0x")
endif()

target_link_libraries(Poker_test -lgmp -lgmpxx -lpthread)
