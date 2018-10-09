#pragma once

#include <chrono>
#include "strategy.h"

#define AB_PRUNING true
#define STATS true

#if STATS
static u64 nodesEvaluated = 0;
static u64 branchNum = 0;
static u64 branchDenom = 0;
#endif

static std::chrono::time_point<std::chrono::system_clock> stopTime;

inline int scorePieces(const Board &board, PieceRange range) {
    int score = 0;

    if (range == PieceRange::White) {
        score += __builtin_popcountll(board.whitePawns.bits) * 10;
        score += __builtin_popcountll(board.whiteKnights.bits) * 40;
        score += __builtin_popcountll(board.whiteRooks.bits) * 35;
        score += __builtin_popcountll(board.whiteBishops.bits) * 30;
        score += (__builtin_ctzll(board.whiteCar.bits) % 8) * 200;
    } else {
        score += __builtin_popcountll(board.blackPawns.bits) * 10;
        score += __builtin_popcountll(board.blackKnights.bits) * 40;
        score += __builtin_popcountll(board.blackRooks.bits) * 35;
        score += __builtin_popcountll(board.blackBishops.bits) * 30;
        score += (__builtin_ctzll(board.blackCar.bits) % 8) * 200;
    }

    return score;
}

int heuristic(const Board &board) {
    int blackScore = scorePieces(board, PieceRange::Black);
    int whiteScore = scorePieces(board, PieceRange::White);
    return blackScore - whiteScore;
}

int minimax(const Board &board, const std::vector<Move> &moves, bool maximizingPlayer, int depth) {
    if (board.getGameState() == GameState::BlackWins) {
        return 10000000 + depth;
    } else if (board.getGameState() == GameState::WhiteWins) {
        return -10000000 - depth;
    } else if (depth == 0 || std::chrono::system_clock::now() > stopTime) {
        return heuristic(board);
    }

    int bestValue = maximizingPlayer? INT_MIN : INT_MAX;

    for (const auto &move : moves) {
        Board boardCopy(board);
        if (maximizingPlayer) {
            boardCopy.performBlackMove(move);
            auto newMoves = boardCopy.getValidMoves(PieceRange::White);
            int nodeValue = minimax(boardCopy, newMoves, !maximizingPlayer, depth - 1);
            if (nodeValue > bestValue) bestValue = nodeValue;
        } else {
            boardCopy.performWhiteMove(move);
            auto newMoves = boardCopy.getValidMoves(PieceRange::Black);
            int nodeValue = minimax(boardCopy, newMoves, !maximizingPlayer, depth - 1);
            if (nodeValue < bestValue) bestValue = nodeValue;
        }
    }

    return bestValue;
}

int alphabeta(const Board &board, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (board.getGameState() == GameState::BlackWins) {
#if STATS
        nodesEvaluated++;
#endif
        return 10000000 + depth;
    } else if (board.getGameState() == GameState::WhiteWins) {
#if STATS
        nodesEvaluated++;
#endif
        return -10000000 - depth;
    } else if (depth == 0 || std::chrono::system_clock::now() > stopTime) {
#if STATS
        nodesEvaluated++;
#endif
        return heuristic(board);
    }

    if (maximizingPlayer) {
        int bestValue = INT_MIN;
        auto moves = board.getValidMoves(PieceRange::Black);
#if STATS
        branchNum += moves.size();
        branchDenom++;
#endif
        for (auto move : moves) {
            Board boardCopy(board);
            boardCopy.performBlackMove(move);
            int nodeValue = alphabeta(boardCopy, depth - 1, alpha, beta, false);
            if (nodeValue > bestValue) bestValue = nodeValue;
            if (nodeValue > alpha) alpha = nodeValue;
            if (alpha >= beta) break;
        }
        return bestValue;
    } else {
        int bestValue = INT_MAX;
        auto moves = board.getValidMoves(PieceRange::White);
#if STATS
        branchNum += moves.size();
        branchDenom++;
#endif
        for (auto move : moves) {
            Board boardCopy(board);
            boardCopy.performWhiteMove(move);
            int nodeValue = alphabeta(boardCopy, depth - 1, alpha, beta, true);
            if (nodeValue < bestValue) bestValue = nodeValue;
            if (nodeValue < beta) beta = nodeValue;
            if (alpha >= beta) break;
        }
        return bestValue;
    }
}

Move getComputerMove(Board &board, std::vector<Move> &moves) {
    Move bestMove{PieceType::EmptyPiece, 0, 0};
    int bestValue = INT_MIN;

#if STATS
    nodesEvaluated = 0;
    branchNum = 0;
    branchDenom = 0;
    auto startTime = std::chrono::steady_clock::now();
#endif

    stopTime = std::chrono::system_clock::now() + std::chrono::seconds(5);

    int depth = 1;
    while (stopTime > std::chrono::system_clock::now()) {
        cout << "Calculating at depth " << ++depth << '\r' << flush;
        for (const auto &move : moves) {
            Board boardCopy(board);
            boardCopy.performBlackMove(move);
#if AB_PRUNING
            int value = alphabeta(boardCopy, depth, INT_MIN, INT_MAX, false);
#else
            int value = minimax(boardCopy, newMoves, false, depth);
#endif
            if (value > bestValue && stopTime > std::chrono::system_clock::now()) {
                bestMove = move;
                bestValue = value;
            }
        }
    }

#if STATS
    auto endTime = std::chrono::steady_clock::now();
    auto diffTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    auto diffTimeSc = diffTimeMs / 1000.0;

    cout << "Evaluated " << nodesEvaluated << " nodes in " << diffTimeMs << "ms." << endl;

    auto nodesInFive = (nodesEvaluated / diffTimeSc) * 5;
    cout << "Nodes in 5: " << nodesInFive << endl;

    auto avgBranches = branchDenom > 0? branchNum / branchDenom : 0;
    cout << "Avg branches: " << avgBranches << endl;
#endif

    return bestMove;
}
