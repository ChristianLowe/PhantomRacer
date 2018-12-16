#pragma once

#include "types.h"
#include "bitboard.h"

class Move {
public:
    PieceType movingPiece;
    u8 fromCell;
    u8 toCell;

    Move inverse() const {
        return Move{movingPiece, inverseShift(fromCell), inverseShift(toCell)};
    }

    PieceRange moveRange() const {
        switch (movingPiece) {
            case PieceType::WhitePawn:
            case PieceType::WhiteKnight:
            case PieceType::WhiteRook:
            case PieceType::WhiteBishop:
            case PieceType::WhiteCar:
                return PieceRange::White;

            case PieceType::BlackPawn:
            case PieceType::BlackKnight:
            case PieceType::BlackRook:
            case PieceType::BlackBishop:
            case PieceType::BlackCar:
                return PieceRange::Black;

            default:
                return PieceRange::NoRange;
        }
    }

    bool operator<(const Move &other) const {
        // Required to use Move as a map key
        unsigned int left = fromCell + (toCell << 7u);
        unsigned int right = other.fromCell + (other.toCell << 7u);
        return left < right;
    }

private:
    u8 inverseShift(u8 cell) const {
        switch (cell / 8) {
            case 0: return static_cast<u8>(cell + 56);
            case 1: return static_cast<u8>(cell + 40);
            case 2: return static_cast<u8>(cell + 24);
            case 3: return static_cast<u8>(cell + 8);
            case 4: return static_cast<u8>(cell - 8);
            case 5: return static_cast<u8>(cell - 24);
            case 6: return static_cast<u8>(cell - 40);
            case 7: return static_cast<u8>(cell - 56);
            default: return 0xFF;
        }
    }
};

class MoveList {
public:
    std::vector<Move> moves;
    u64 carIdx = 0;

    MoveList(std::vector<Move> moveList) : moves(moveList) {}
};

std::ostream& operator<<(std::ostream &stream, const Move &move) {
    return stream
            << static_cast<char>('A' + move.fromCell % 8)
            << static_cast<char>('1' + move.fromCell / 8)
            << static_cast<char>('A' + move.toCell % 8)
            << static_cast<char>('1' + move.toCell / 8);
}

void operator>>(char* input, Move &move) {
    // Read move in and make it upper-case
    std::string str(input);
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);

    // Set move relative to ASCII codes used
    move.fromCell = static_cast<u8>((str[1] - '1') * 8 + (str[0] - 'A'));
    move.toCell = static_cast<u8>((str[3] - '1') * 8 + (str[2] - 'A'));
}

inline bool operator==(const volatile Move &lhs, const Move &rhs) {
    return lhs.fromCell == rhs.fromCell && lhs.toCell == rhs.toCell;
}

