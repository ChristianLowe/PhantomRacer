#pragma once

#include <random>

#include "strategy.h"

Move getComputerMove(Board &board, MoveList &moves) {
    return moves.moves[rand() % moves.moves.size()]; // NOLINT(cert-msc50-c,cert-msc30-c,cert-msc50-cpp)
}
