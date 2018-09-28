#include <iostream>

#include "board.h"
#include "intro.h"
#include "test.h"
#include "move.h"

// Strategies available: random, minimax
#include "strategy/minimax.h"

using std::cin;
using std::cout;
using std::endl;
using std::flush;

void gameMain();
Move getPlayerMove(const std::vector<Move> &moves);

int main() {
#if TESTING
    testingMain();
#else
    gameMain();
#endif
    return 0;
}

void gameMain() {
    showIntroText();

    PieceRange currentPlayer = getStartingParticipant();

    Board board;

    cout << endl << "Welcome! Here's a new board:" << endl;
    while (true) {
        auto moves = board.getValidMoves(currentPlayer);
        cout << board << endl;

        GameState gameState = board.getGameState();
        if (gameState != GameState::IsPlaying) {
            std::string winner = gameState == GameState::HumanWins? "player" : "computer";
            cout << "Game over! The " << winner << " won." << endl;
            return;
        }

        Move move{};
        if (currentPlayer == PieceRange::CpuAi) {
            move = getComputerMove(board, moves);
            currentPlayer = PieceRange::Human;
        } else {
            move = getPlayerMove(moves);
            currentPlayer = PieceRange::CpuAi;
        }

        cout << "Move: " << move << '(' << move.inverse() << ')' << endl;
        board.performMove(move);
    }
}

Move getPlayerMove(const std::vector<Move> &moves) {
    while (true) {
        Move playerMove{};

        cout << endl << "Valid moves: ";
        for (auto move : moves) {
            cout << move << ' ';
        }
        cout << endl;
        cout << "What's your move? " << flush;
        cin >> playerMove;

        for (auto possibleMove : moves) {
            if (playerMove == possibleMove) return possibleMove;
        }

        cout << "Sorry, you cannot make that move. Try again." << endl;
    }
}
