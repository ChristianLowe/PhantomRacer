#pragma once

#include <iostream>

#include "types.h"
#include "color.h"
#include "move.h"
#include "game.h"

using std::endl;

enum PieceType {
    EmptyPiece  = 00,

    CpuAiPawn   = 11,
    CpuAiBishop = 13,
    CpuAiRook   = 15,
    CpuAiKnight = 17,
    CpuAiCar    = 19,

    HumanPawn   = 21,
    HumanBishop = 23,
    HumanRook   = 25,
    HumanKnight = 27,
    HumanCar    = 29,
};

static const u8 STARTING_BOARD[8][7] = {
        {19, 00, 00, 00, 00, 00, 00},
        {00, 11, 00, 00, 00, 00, 00},
        {15, 15, 11, 13, 13, 00, 00},
        {17, 17, 00, 11, 11, 11, 11},
        {27, 27, 00, 21, 21, 21, 21},
        {25, 25, 21, 23, 23, 00, 00},
        {00, 21, 00, 00, 00, 00, 00},
        {29, 00, 00, 00, 00, 00, 00},
};

static const bool LANE_SQUARES[8][7] = {
        {true , false, false, false, false, false, false},
        {false, true , false, false, false, false, false},
        {false, false, true , false, false, false, false},
        {false, false, false, true , true , true , true },
        {false, false, false, true , true , true , true },
        {false, false, true , false, false, false, false},
        {false, true , false, false, false, false, false},
        {true , false, false, false, false, false, false},
};

class Board {
    u8 *memory;

    inline bool inBounds(u8 x, u8 y) const {
        return x < 7 && y < 8;
    }

    inline bool inRange(int piece, PieceRange range) const {
        return range == PieceRange::Human? piece > PieceRange::Human : piece < PieceRange::Human;
    }

    inline bool rayCast(std::vector<Move> &moves, PieceRange range, u8 x, u8 y,
                        int forwardY, int forwardX, bool canGoEmpty) const {
        u8 testX = x;
        u8 testY = y;

        do {
            // Forward left
            testX += forwardX;
            testY += forwardY;

            if(inBounds(testX, testY) && isEmpty(testX, testY)) {
                if (canGoEmpty) moves.push_back(Move {x, y, testX, testY});
            } else break;
        } while (true);

        if (inBounds(testX, testY)) {
            int piece = getPiece(testX, testY);
            if (!inRange(piece, range) && piece != CpuAiCar && piece != HumanCar) {
                moves.push_back(Move {x, y, testX, testY});
            }
        }
    }

    inline void testAndAdd(std::vector<Move> &moves, PieceRange range,
            u8 x, u8 y, int relX, int relY, bool canGoEmpty) const {
        auto testX = static_cast<u8>(x + relX);
        auto testY = static_cast<u8>(y + relY);

        if (inBounds(testX, testY)) {
            u8 targetPiece = getPiece(testX, testY);
            if ((targetPiece == EmptyPiece && canGoEmpty)
             || (targetPiece != EmptyPiece && targetPiece != HumanCar
                && targetPiece != CpuAiCar && !inRange(targetPiece, range))) {
                moves.push_back(Move {x, y, testX, testY});
            }
        }
    }

    inline void addCarMove(std::vector<Move> &moves, PieceRange range, bool necessary) const {
        Move carMove = getCarMove(range);
        if (necessary || isEmpty(carMove.x2, carMove.y2)) {
            moves.push_back(carMove);
        }
    }

    void addValidMoves(std::vector<Move> &moves, PieceRange range, u8 x, u8 y, int forwardY) const {
        switch(getPiece(x, y) % 10) {
            case 1: // Pawn
                if (inBounds(x, static_cast<u8>(y + forwardY)) && isEmpty(x, static_cast<u8>(y + forwardY))) {
                    moves.push_back(Move {x, y, x, static_cast<u8>(y + forwardY)});
                }

                testAndAdd(moves, range, x, y, +1, forwardY, false);
                testAndAdd(moves, range, x, y, -1, forwardY, false);
                break;

            case 3: // Bishop
                rayCast(moves, range, x, y, +forwardY, -1, true); // Forward left
                rayCast(moves, range, x, y, +forwardY, +1, true); // Forward right
                rayCast(moves, range, x, y, -forwardY, -1, false); // Backwards left
                rayCast(moves, range, x, y, -forwardY, +1, false); // Backwards right
                break;

            case 5: // Rook
                rayCast(moves, range, x, y, +forwardY, 0, true); // Forward
                rayCast(moves, range, x, y, -forwardY, 0, false); // Backward
                rayCast(moves, range, x, y, 0, -1, false); // Left
                rayCast(moves, range, x, y, 0, +1, false); // Right
                break;

            case 7: // Knight
                // Forward
                testAndAdd(moves, range, x, y, 2, forwardY, true);
                testAndAdd(moves, range, x, y, -2, forwardY, true);
                testAndAdd(moves, range, x, y, 1, forwardY*2, true);
                testAndAdd(moves, range, x, y, -1, forwardY*2, true);

                // Backward
                testAndAdd(moves, range, x, y, 2, -forwardY, false);
                testAndAdd(moves, range, x, y, -2, -forwardY, false);
                testAndAdd(moves, range, x, y, 1, (-forwardY)*2, false);
                testAndAdd(moves, range, x, y, -1, (-forwardY)*2, false);

                break;

            case 9: // The Car
                addCarMove(moves, range, false);
                break;

            default:
                std::cout << "unknown piece (x: " << x << ", y: " << y <<
                            ") (piece type: " << getPiece(x,y) << ")" << std::endl;
                break;
        }
    }

public:
    Board() {
        memory = new u8[8 * 7];

        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 7; x++) {
                setPiece(x, y, STARTING_BOARD[y][x]);
            }
        }
    }

    Board(const Board& from) {
        memory = new u8[8 * 7];

        for (int i = 0; i < 8 * 7; i++) {
            memory[i] = from.memory[i];
        }
    }

    ~Board() {
        delete[](memory);
    }

    void performMove(const Move &move) {
        u8 oldPiece = getPiece(move.x1, move.y1);
        setPiece(move.x1, move.y1, 00);
        setPiece(move.x2, move.y2, oldPiece);
    }

    inline Move getCarMove(PieceRange range) const {
        if (range == PieceRange::CpuAi) {
            if (getPiece(0, 0) == CpuAiCar) return Move {0, 0, 1, 1};
            if (getPiece(1, 1) == CpuAiCar) return Move {1, 1, 2, 2};
            if (getPiece(2, 2) == CpuAiCar) return Move {2, 2, 3, 3};
            if (getPiece(3, 3) == CpuAiCar) return Move {3, 3, 4, 3};
            if (getPiece(4, 3) == CpuAiCar) return Move {4, 3, 5, 3};
            if (getPiece(5, 3) == CpuAiCar) return Move {5, 3, 6, 3};
        } else {
            if (getPiece(0, 7) == HumanCar) return Move {0, 7, 1, 6};
            if (getPiece(1, 6) == HumanCar) return Move {1, 6, 2, 5};
            if (getPiece(2, 5) == HumanCar) return Move {2, 5, 3, 4};
            if (getPiece(3, 4) == HumanCar) return Move {3, 4, 4, 4};
            if (getPiece(4, 4) == HumanCar) return Move {4, 4, 5, 4};
            if (getPiece(5, 4) == HumanCar) return Move {5, 4, 6, 4};
        }
        std::cout << "??? Dude, where's my car ???" << std::endl;
    }

    std::vector<Move> getValidMoves(PieceRange range) const {
        // TODO: Zobrist hashing

        auto moves = std::vector<Move>();
        int forwardY = range == PieceRange::CpuAi ? 1 : -1; // The computer considers down (+y) as "forward"

        for (u8 y = 0; y < 8; y++) {
            for (u8 x = 0; x < 7; x++) {
                int pieceType = getPiece(x, y) - range;
                if (pieceType >= 0 && pieceType <= 9) {
                    addValidMoves(moves, range, x, y, forwardY);
                }
            }
        }

        if (moves.empty()) {
            addCarMove(moves, range, true);
        }

        return moves;
    }

    GameState getGameState() const {
        if (getPiece(6, 3) == CpuAiCar) return GameState::CpuAiWins;
        if (getPiece(6, 4) == HumanCar) return GameState::HumanWins;
        return GameState::IsPlaying;
    }

    inline bool isEmpty(int x, int y) const {
        return getPiece(x, y) == 00;
    }

    inline u8 getPiece(int x, int y) const {
        return memory[x + y * 7];
    }

    inline void setPiece(int x, int y, u8 piece) {
        memory[x + y * 7] = piece;
    }
};

std::string pieceToString(u8 piece) {
    const bool human = piece > PieceRange::Human;

    switch (piece % 10) {
        case 0:
            return "   ";
        case 1:
            return human? " p " : " P ";
        case 3:
            return human? " b " : " B ";
        case 5:
            return human? " r " : " R ";
        case 7:
            return human? " n " : " N ";
        case 9:
            return human? " c " : " C ";
        default:
            return " ? ";
    }
}

std::ostream& operator<<(std::ostream &stream, const Board &board) {
    bool blueBG = true;

    stream << "   --------------------- COMPUTER" << endl;

    for (int y = 0; y < 8; y++) {
        stream << ' ' << 8 - y << ' ';

        for (int x = 0; x < 7; x++) {
            u8 piece = board.getPiece(x, y);

            int fgColor = piece > PieceRange::Human? WHITE : GREEN;
            int bgColor = LANE_SQUARES[y][x]? RED : (blueBG? BLUE : BLACK);
            setColor(stream, BRIGHT, fgColor, bgColor);
            stream << pieceToString(piece);

            blueBG = !blueBG;
        }

        resetColor(stream);
        stream << endl;
    }

    resetColor(stream);
    stream << "   --------------------- HUMAN" << endl;
    stream << "    A  B  C  D  E  F  G " << endl << endl;

    return stream;
}

