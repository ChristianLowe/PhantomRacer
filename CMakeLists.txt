cmake_minimum_required(VERSION 3.12)
project(phantomracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(phantomracer main.cpp board.h color.h move.h types.h game.h test.h intro.h strategy/random.h strategy/strategy.h strategy/minimax.h)