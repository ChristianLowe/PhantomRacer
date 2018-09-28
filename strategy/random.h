#pragma once

#include <random>

#include "strategy.h"

Move getComputerMove(Board &board, std::vector<Move> &moves) {
    return moves[rand() % moves.size()]; // NOLINT(cert-msc50-c,cert-msc30-c,cert-msc50-cpp)
}
