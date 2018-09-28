#pragma once

#include "strategy.h"

#define AB_PRUNING false

int scorePieces(const Board &board, PieceRange range, int depth) {
    int score = 0;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 7; x++) {
            int pieceType = board.getPiece(x, y) - range;
            if (pieceType >= 0 && pieceType <= 9) {
                if (range == PieceRange::CpuAi) {
                    score += 8 - y;
                } else {
                    score += y;
                }

                switch (pieceType) {
                    case 1: // Pawn
                        score += 10;
                        break;
                    case 3: // Bishop
                        score += 30;
                        break;
                    case 5: // Rook
                        score += 35;
                        break;
                    case 7: // Knight
                        score += 40;
                        break;
                    case 9: // Car
                        if (x == 6) {
                            score += 100000 - (depth * 1000);
                        } else {
                            score += x * 200;
                        }
                        break;
                    default:
                        cout << "** scoring unknown piece?" << endl;
                }
            }
        }
    }

    return score;
}

int heuristic(const Board &board, int depth) {
    int cpuAiScore = scorePieces(board, PieceRange::CpuAi, depth);
    int humanScore = scorePieces(board, PieceRange::Human, depth);
    return cpuAiScore - humanScore;
}

int minimax(const Board &board, const std::vector<Move> &moves, bool maximizingPlayer, int depth) {
    if (depth == 0 || board.getGameState() != GameState::IsPlaying) {
        return heuristic(board, depth);
    }

    std::function<int(int, int)> betterOf;
    int value;

    if (maximizingPlayer) {
        betterOf = [](int a, int b) { return a > b ? a : b; };
        value = INT_MIN;
    } else {
        betterOf = [](int a, int b) { return a < b ? a : b; };
        value = INT_MAX;
    }

    for (const auto &move : moves) {
        Board boardCopy(board);
        boardCopy.performMove(move);
        auto newMoves = boardCopy.getValidMoves(PieceRange::CpuAi);

        value = betterOf(value, minimax(boardCopy, newMoves, !maximizingPlayer, depth - 1));
    }

    return value;
}

int alphabeta(const Board &board, const std::vector<Move> &moves, bool maximizingPlayer, int depth,
        int min, int max) {

    if (depth == 0 || board.getGameState() != GameState::IsPlaying) {
        return heuristic(board, depth);
    }

    int value = maximizingPlayer? min : max;

    for (const auto &move : moves) {
        Board boardCopy(board);
        boardCopy.performMove(move);
        auto newMoves = boardCopy.getValidMoves(PieceRange::CpuAi);

        if (maximizingPlayer) {
            int childValue = alphabeta(boardCopy, newMoves, false, depth - 1, value, max);
            if (childValue > value)
                value = childValue;
            if (value > max)
                return max;
        } else {
            int childValue = alphabeta(boardCopy, newMoves, true, depth - 1, min, value);
            if (childValue < value)
                value = childValue;
            if (value < min)
                return min;
        }
    }

    return value;
}

Move getComputerMove(Board &board, std::vector<Move> &moves) {
    Move bestMove{};
    int bestValue = INT_MIN;

    for (const auto &move : moves) {
        Board boardCopy(board);
        boardCopy.performMove(move);
        auto newMoves = boardCopy.getValidMoves(PieceRange::CpuAi);

#if AB_PRUNING
        int value = alphabeta(boardCopy, newMoves, false, 5, INT_MIN, INT_MAX);
#else
        int value = minimax(boardCopy, newMoves, false, 5);
#endif
        if (value > bestValue) {
            bestMove = move;
            bestValue = value;
        }
    }

    return bestMove;
}
