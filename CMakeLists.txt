cmake_minimum_required(VERSION 3.12)
project(phantomracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-march=native -Ofast -funroll-loops -Wall -Wextra")

add_executable(phantomracer main.cpp color.h types.h game.h test.h intro.h strategy/random.h strategy/strategy.h strategy/minimax.h bitboard.h board.h move.h)
