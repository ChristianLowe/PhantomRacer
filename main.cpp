#include <iostream>

#include "bitboard.h"
#include "intro.h"
#include "game.h"
#include "test.h"
#include "board.h"
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
    initPieceLookupTable();
    initKnightLookupTable();
    initRayLookupTable();

    showIntroText();

    Board board;
    PieceRange currentPlayer = getStartingParticipant();

    cout << endl << "Welcome! Here's a new board:" << endl;
    while (true) {
        auto moves = board.getValidMoves(currentPlayer);
        cout << board << endl;

        GameState gameState = board.getGameState();
        if (gameState != GameState::IsPlaying) {
            std::string winner = gameState == GameState::WhiteWins? "player" : "computer";
            cout << "Game over! The " << winner << " won." << endl;
            return;
        }

        Move move{PieceType::EmptyPiece, 0, 0};
        if (currentPlayer == PieceRange::Black) {
            move = getComputerMove(board, moves);
            board.performBlackMove(move);
            currentPlayer = PieceRange::White;
        } else {
            move = getPlayerMove(moves);
            board.performWhiteMove(move);
            currentPlayer = PieceRange::Black;
        }

        cout << "Move: " << move << '(' << move.inverse() << ')' << endl;
    }
}

Move getPlayerMove(const std::vector<Move> &moves) {
    while (true) {
        Move playerMove{PieceType::EmptyPiece, 0, 0};

        cout << endl << "Valid moves: ";
        for (auto move : moves) {
            cout << move << ' ';
        }
        cout << endl << "What's your move? " << flush;

        char inputBuffer[5] = {0};
        cin >> inputBuffer;
        inputBuffer[4] = '\0';
        inputBuffer >> playerMove;

        for (auto possibleMove : moves) {
            if (playerMove == possibleMove) return possibleMove;
        }

        cout << "Sorry, you cannot make that move. Try again." << endl;
    }
}
