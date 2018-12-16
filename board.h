#pragma once

#include <ostream>
#include <vector>
#include <unordered_map>

#include "types.h"
#include "bitboard.h"
#include "color.h"
#include "move.h"

using std::endl;

//static std::unordered_map<u64, std::vector<Move>> zobristWhiteMap;
//static std::unordered_map<u64, std::vector<Move>> zobristBlackMap;

class Board {
public:
    BitBoard whitePawns     = 0b0000000000000000000000000000000001111000000001000000001000000000;
    BitBoard whiteKnights   = 0b0000000000000000000000000000000000000011000000000000000000000000;
    BitBoard whiteRooks     = 0b0000000000000000000000000000000000000000000000110000000000000000;
    BitBoard whiteBishops   = 0b0000000000000000000000000000000000000000000110000000000000000000;
    BitBoard whiteCar       = 0b0000000000000000000000000000000000000000000000000000000000000001;

    BitBoard blackPawns     = 0b0000000000000010000001000111100000000000000000000000000000000000;
    BitBoard blackKnights   = 0b0000000000000000000000000000001100000000000000000000000000000000;
    BitBoard blackRooks     = 0b0000000000000000000000110000000000000000000000000000000000000000;
    BitBoard blackBishops   = 0b0000000000000000000110000000000000000000000000000000000000000000;
    BitBoard blackCar       = 0b0000000100000000000000000000000000000000000000000000000000000000;

    BitBoard allWhitePieces;
    BitBoard allBlackPieces;
    BitBoard allPieces;

    Board() {
        updatePieceAggregates();
    }

    Board(const Board &oldBoard) {
        whitePawns = oldBoard.whitePawns;
        whiteRooks = oldBoard.whiteRooks;
        whiteKnights = oldBoard.whiteKnights;
        whiteBishops = oldBoard.whiteBishops;
        whiteCar = oldBoard.whiteCar;

        blackPawns = oldBoard.blackPawns;
        blackRooks = oldBoard.blackRooks;
        blackKnights = oldBoard.blackKnights;
        blackBishops = oldBoard.blackBishops;
        blackCar = oldBoard.blackCar;

        updatePieceAggregates();
    }

    inline void updatePieceAggregates() {
        allWhitePieces = whitePawns.bits | whiteRooks.bits | whiteKnights.bits | whiteBishops.bits | whiteCar.bits;
        allBlackPieces = blackPawns.bits | blackRooks.bits | blackKnights.bits | blackBishops.bits | blackCar.bits;
        allPieces = allWhitePieces.bits | allBlackPieces.bits;
    }

    inline MoveList getValidMoves(PieceRange range, bool includeCar = true) const {
        if (range == PieceRange::White) {
            std::vector<Move> moveVector;
            moveVector.reserve(32);

            MoveList moves(moveVector);
            addWhitePawnMoves(moves);
            addWhiteKnightMoves(moves);
            addWhiteRookMoves(moves);
            addWhiteBishopMoves(moves);
            if (includeCar) addWhiteCarMove(moves);

            return moves;
        } else {
            std::vector<Move> moveVector;
            moveVector.reserve(32);

            MoveList moves(moveVector);
            addBlackPawnMoves(moves);
            addBlackKnightMoves(moves);
            addBlackRookMoves(moves);
            addBlackBishopMoves(moves);
            if (includeCar) addBlackCarMove(moves);

            return moves;
        }
    }

    GameState getGameState() const {
        if (unlikely(whiteCar.bits == pieceLookupTable[30])) {
            return GameState::WhiteWins;
        } else if (unlikely(blackCar.bits == pieceLookupTable[38])) {
            return GameState::BlackWins;
        } else {
            return GameState::IsPlaying;
        }
    }

    void performWhiteMove(Move move) {
        u64 fromMask = maskPieceLookupTable[move.fromCell];
        u64 toMask = maskPieceLookupTable[move.toCell];

        switch (move.movingPiece) {
            case WhitePawn:
                whitePawns.bits &= fromMask;
                whitePawns.bits |= pieceLookupTable[move.toCell];
                break;
            case WhiteKnight:
                whiteKnights.bits &= fromMask;
                whiteKnights.bits |= pieceLookupTable[move.toCell];
                break;
            case WhiteRook:
                whiteRooks.bits &= fromMask;
                whiteRooks.bits |= pieceLookupTable[move.toCell];
                break;
            case WhiteBishop:
                whiteBishops.bits &= fromMask;
                whiteBishops.bits |= pieceLookupTable[move.toCell];
                break;
            case WhiteCar:
                whitePawns.bits &= toMask;
                whiteKnights.bits &= toMask;
                whiteRooks.bits &= toMask;
                whiteBishops.bits &= toMask;
                whiteCar.bits = pieceLookupTable[move.toCell];
                break;
            default:
                cout << "Invalid piece type for white move" << endl;
        }

        blackPawns.bits &= toMask;
        blackKnights.bits &= toMask;
        blackRooks.bits &= toMask;
        blackBishops.bits &= toMask;

        updatePieceAggregates();
    }

    void performBlackMove(Move move) {
        u64 fromMask = maskPieceLookupTable[move.fromCell];
        u64 toMask = maskPieceLookupTable[move.toCell];

        switch (move.movingPiece) {
            case BlackPawn:
                blackPawns.bits &= fromMask;
                blackPawns.bits |= pieceLookupTable[move.toCell];
                break;
            case BlackKnight:
                blackKnights.bits &= fromMask;
                blackKnights.bits |= pieceLookupTable[move.toCell];
                break;
            case BlackRook:
                blackRooks.bits &= fromMask;
                blackRooks.bits |= pieceLookupTable[move.toCell];
                break;
            case BlackBishop:
                blackBishops.bits &= fromMask;
                blackBishops.bits |= pieceLookupTable[move.toCell];
                break;
            case BlackCar:
                blackPawns.bits &= toMask;
                blackKnights.bits &= toMask;
                blackRooks.bits &= toMask;
                blackBishops.bits &= toMask;
                blackCar.bits = pieceLookupTable[move.toCell];
                break;
            default:
                cout << "Invalid piece type for black move" << endl;
        }

        whitePawns.bits &= toMask;
        whiteKnights.bits &= toMask;
        whiteRooks.bits &= toMask;
        whiteBishops.bits &= toMask;

        updatePieceAggregates();
    }

    u64 hash() const {
        u64 result = 0;
        u64 bits;

        for (int i = 1; i <= 10; i++) {
            bits = pieceTypeToBoard(static_cast<PieceType>(i)).bits;
            while (bits) {
                auto firstBit = static_cast<u8>(__builtin_ctzll(bits));
                result ^= zobristTable[i][firstBit];
                bits &= bits - 1;
            }
        }

        return result;
    }

private:
    void addWhitePawnMoves(MoveList &moves) const {
        BitBoard pawnBoard;

        pawnBoard.bits = whitePawns.bits << 9u & leftColMask & allBlackPieces.bits & ~blackCar.bits;
        while (pawnBoard.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(pawnBoard.bits));
            moves.moves.push_back(Move{WhitePawn, static_cast<u8>(firstBit - 9), firstBit});
            pawnBoard.bits &= pawnBoard.bits - 1;
        }

        pawnBoard.bits = whitePawns.bits << 7u & rightColMask & allBlackPieces.bits & ~blackCar.bits;
        while (pawnBoard.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(pawnBoard.bits));
            moves.moves.push_back(Move{WhitePawn, static_cast<u8>(firstBit - 7), firstBit});
            pawnBoard.bits &= pawnBoard.bits - 1;
        }

        pawnBoard.bits = whitePawns.bits << 8u & ~allPieces.bits;
        while (pawnBoard.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(pawnBoard.bits));
            moves.moves.push_back(Move{WhitePawn, static_cast<u8>(firstBit - 8), firstBit});
            pawnBoard.bits &= pawnBoard.bits - 1;
        }
    }

    void addBlackPawnMoves(MoveList &moves) const {
        BitBoard pawnBoard;

        pawnBoard.bits = blackPawns.bits >> 9u & rightColMask & allWhitePieces.bits & ~whiteCar.bits;
        while (pawnBoard.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(pawnBoard.bits));
            moves.moves.push_back(Move{BlackPawn, static_cast<u8>(firstBit + 9), firstBit});
            pawnBoard.bits &= pawnBoard.bits - 1;
        }

        pawnBoard.bits = blackPawns.bits >> 7u & leftColMask & allWhitePieces.bits & ~whiteCar.bits;
        while (pawnBoard.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(pawnBoard.bits));
            moves.moves.push_back(Move{BlackPawn, static_cast<u8>(firstBit + 7), firstBit});
            pawnBoard.bits &= pawnBoard.bits - 1;
        }

        pawnBoard.bits = blackPawns.bits >> 8u & ~allPieces.bits;
        while (pawnBoard.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(pawnBoard.bits));
            moves.moves.push_back(Move{BlackPawn, static_cast<u8>(firstBit + 8), firstBit});
            pawnBoard.bits &= pawnBoard.bits - 1;
        }
    }

    void addWhiteKnightMoves(MoveList &moves) const {
        BitBoard knights(whiteKnights);
        while (knights.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(knights.bits));
            knights.bits &= knights.bits - 1;

            BitBoard attack = knightLookupTable[firstBit] & ~(allWhitePieces.bits | blackCar.bits);

            while (attack.bits) {
                auto secondBit = static_cast<u8>(__builtin_ctzll(attack.bits));
                if (secondBit > firstBit) {
                    moves.moves.push_back(Move{WhiteKnight, firstBit, secondBit});
                } else {
                    u64 testBit = C64(1) << secondBit;
                    if ((testBit & allBlackPieces.bits) && (testBit ^ blackCar.bits)) {
                        moves.moves.push_back(Move{WhiteKnight, firstBit, secondBit});
                    }
                }
                attack.bits &= attack.bits - 1;
            }
        }
    }

    void addBlackKnightMoves(MoveList &moves) const {
        BitBoard knights(blackKnights);
        while (knights.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(knights.bits));
            knights.bits &= knights.bits - 1;

            BitBoard attack = knightLookupTable[firstBit] & ~(allBlackPieces.bits | whiteCar.bits);

            while (attack.bits) {
                auto secondBit = static_cast<u8>(__builtin_ctzll(attack.bits));
                if (secondBit < firstBit) {
                    moves.moves.push_back(Move{BlackKnight, firstBit, secondBit});
                } else {
                    u64 testBit = C64(1) << secondBit;
                    if ((testBit & allWhitePieces.bits) && (testBit ^ whiteCar.bits)) {
                        moves.moves.push_back(Move{BlackKnight, firstBit, secondBit});
                    }
                }
                attack.bits &= attack.bits - 1;
            }
        }
    }

    void addWhiteRookMoves(MoveList &moves) const {
        BitBoard rooks(whiteRooks);
        while (rooks.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(rooks.bits));
            rooks.bits &= rooks.bits - 1;

            BitBoard attack = getPositiveRayAttacks(allWhitePieces, North, firstBit);
            attack.bits |= (getPositiveRayAttacks(allWhitePieces, East, firstBit)
                            | getNegativeRayAttacks(allWhitePieces, West, firstBit)
                            | getNegativeRayAttacks(allWhitePieces, South, firstBit)) & allBlackPieces.bits;
            attack.bits &= ~blackCar.bits;


            while (attack.bits) {
                auto secondBit = static_cast<u8>(__builtin_ctzll(attack.bits));
                moves.moves.push_back(Move{WhiteRook, firstBit, secondBit});
                attack.bits &= attack.bits - 1;
            }
        }
    }

    void addBlackRookMoves(MoveList &moves) const {
        BitBoard rooks(blackRooks);
        while (rooks.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(rooks.bits));
            rooks.bits &= rooks.bits - 1;

            BitBoard attack = getNegativeRayAttacks(allBlackPieces, South, firstBit);
            attack.bits |= (getPositiveRayAttacks(allBlackPieces, East, firstBit)
                            | getNegativeRayAttacks(allBlackPieces, West, firstBit)
                            | getPositiveRayAttacks(allBlackPieces, North, firstBit)) & allWhitePieces.bits;
            attack.bits &= ~whiteCar.bits;

            while (attack.bits) {
                auto secondBit = static_cast<u8>(__builtin_ctzll(attack.bits));
                moves.moves.push_back(Move{BlackRook, firstBit, secondBit});
                attack.bits &= attack.bits - 1;
            }
        }
    }

    void addWhiteBishopMoves(MoveList &moves) const {
        BitBoard bishops(whiteBishops);
        while (bishops.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(bishops.bits));
            bishops.bits &= bishops.bits - 1;

            BitBoard attack = getPositiveRayAttacks(allWhitePieces, NorthEast, firstBit)
                            | getPositiveRayAttacks(allWhitePieces, NorthWest, firstBit);
            attack.bits |=  ( getNegativeRayAttacks(allWhitePieces, SouthEast, firstBit)
                            | getNegativeRayAttacks(allWhitePieces, SouthWest, firstBit)) & allBlackPieces.bits;
            attack.bits &= ~blackCar.bits;


            while (attack.bits) {
                auto secondBit = static_cast<u8>(__builtin_ctzll(attack.bits));
                moves.moves.push_back(Move{WhiteBishop, firstBit, secondBit});
                attack.bits &= attack.bits - 1;
            }
        }
    }

    void addBlackBishopMoves(MoveList &moves) const {
        BitBoard bishops(blackBishops);
        while (bishops.bits) {
            auto firstBit = static_cast<u8>(__builtin_ctzll(bishops.bits));
            bishops.bits &= bishops.bits - 1;

            BitBoard attack = getNegativeRayAttacks(allBlackPieces, SouthEast, firstBit)
                            | getNegativeRayAttacks(allBlackPieces, SouthWest, firstBit);
            attack.bits |=  ( getPositiveRayAttacks(allBlackPieces, NorthEast, firstBit)
                            | getPositiveRayAttacks(allBlackPieces, NorthWest, firstBit)) & allWhitePieces.bits;
            attack.bits &= ~whiteCar.bits;


            while (attack.bits) {
                auto secondBit = static_cast<u8>(__builtin_ctzll(attack.bits));
                moves.moves.push_back(Move{BlackBishop, firstBit, secondBit});
                attack.bits &= attack.bits - 1;
            }
        }
    }

    void addWhiteCarMove(MoveList &moves) const {
        Move move{WhiteCar, 0, 0};
        switch(whiteCar.bits) {
            case C64(0b0000000000000000000000000000000000000000000000000000000000000001):
                move = Move{WhiteCar, 0, 9};
                break;
            case C64(0b0000000000000000000000000000000000000000000000000000001000000000):
                move = Move{WhiteCar, 9, 18};
                break;
            case C64(0b0000000000000000000000000000000000000000000001000000000000000000):
                move = Move{WhiteCar, 18, 27};
                break;
            case C64(0b0000000000000000000000000000000000001000000000000000000000000000):
                move = Move{WhiteCar, 27, 28};
                break;
            case C64(0b0000000000000000000000000000000000010000000000000000000000000000):
                move = Move{WhiteCar, 28, 29};
                break;
            case C64(0b0000000000000000000000000000000000100000000000000000000000000000):
                move = Move{WhiteCar, 29, 30};
                break;
            default:
                cout << "?? Dude, where's my car ??" << endl;
        }

        if ((allPieces.bits & pieceLookupTable[move.toCell]) == 0 || moves.moves.empty()) {
            moves.carIdx = moves.moves.size();
            moves.moves.push_back(move);
        }
    }

    void addBlackCarMove(MoveList &moves) const {
        // C++ doesn't support switch-case for values outside the range of an int, so we use if-else instead.
        Move move{BlackCar, 0, 0};
        if (blackCar.bits & pieceLookupTable[56]) {
            move = Move{BlackCar, 56, 49};
        } else if (blackCar.bits & pieceLookupTable[49]) {
            move = Move{BlackCar, 49, 42};
        } else if (blackCar.bits & pieceLookupTable[42]) {
            move = Move{BlackCar, 42, 35};
        } else if (blackCar.bits & pieceLookupTable[35]) {
            move = Move{BlackCar, 35, 36};
        } else if (blackCar.bits & pieceLookupTable[36]) {
            move = Move{BlackCar, 36, 37};
        } else if (blackCar.bits & pieceLookupTable[37]) {
            move = Move{BlackCar, 37, 38};
        } else {
            cout << "?? Dude, where's my car ??" << endl;
        }

        if ((allPieces.bits & pieceLookupTable[move.toCell]) == 0 || moves.moves.empty()) {
            moves.carIdx = moves.moves.size();
            moves.moves.push_back(move);
        }
    }

    inline u64 getPositiveRayAttacks(BitBoard friendly, ScanDirection direction, u8 square) const {
        u64 attacks = rayLookupTable[direction][square];
        u64 blocker = attacks & allPieces.bits;
        square = static_cast<u8>(__builtin_ctzll(blocker | C64(0x8000000000000000)));
        u64 friendMask = friendly.bits & pieceLookupTable[square];
        return attacks ^ rayLookupTable[direction][square] ^ friendMask;
    }

    inline u64 getNegativeRayAttacks(BitBoard friendly, ScanDirection direction, u8 square) const {
        u64 attacks = rayLookupTable[direction][square];
        u64 blocker = attacks & allPieces.bits;
        square = static_cast<u8>(63 - __builtin_clzll(blocker | C64(1)));
        u64 friendMask = friendly.bits & pieceLookupTable[square];
        return attacks ^ rayLookupTable[direction][square] ^ friendMask;
    }

    BitBoard pieceTypeToBoard(PieceType pieceType) const {
        switch (pieceType) {
            case BlackPawn:     return blackPawns;
            case BlackKnight:   return blackKnights;
            case BlackRook:     return blackRooks;
            case BlackBishop:   return blackBishops;
            case BlackCar:      return blackCar;

            case WhitePawn:     return whitePawns;
            case WhiteKnight:   return whiteKnights;
            case WhiteRook:     return whiteRooks;
            case WhiteBishop:   return whiteBishops;
            case WhiteCar:      return whiteCar;

            default:
                cout << "Invalid board lookup from pieceType" << endl;
                return {};
        }
    }
};

static const bool CAR_SQUARES[8][7] = {
        {true , false, false, false, false, false, false},
        {false, true , false, false, false, false, false},
        {false, false, true , false, false, false, false},
        {false, false, false, true , true , true , true },
        {false, false, false, true , true , true , true },
        {false, false, true , false, false, false, false},
        {false, true , false, false, false, false, false},
        {true , false, false, false, false, false, false},
};

std::ostream& operator<<(std::ostream &stream, const Board &board) {
    bool blueBG = true;

    stream << "   --------------------- COMPUTER" << endl;

    for (int y = 0; y < 8; y++) {
        stream << ' ' << 8 - y << ' ';

        for (int x = 0; x < 7; x++) {
            int bgColor = CAR_SQUARES[y][x]? RED : (blueBG? BLUE : BLACK);

            if (board.allPieces.isBitSet(x, y)) {
                if (board.allWhitePieces.isBitSet(x, y)) {
                    setColor(stream, BRIGHT, WHITE, bgColor);
                    if (board.whitePawns.isBitSet(x, y))    stream << " p ";
                    if (board.whiteRooks.isBitSet(x, y))    stream << " r ";
                    if (board.whiteKnights.isBitSet(x, y))  stream << " n ";
                    if (board.whiteBishops.isBitSet(x, y))  stream << " b ";
                    if (board.whiteCar.isBitSet(x, y))      stream << " c ";
                } else {
                    setColor(stream, BRIGHT, GREEN, bgColor);
                    if (board.blackPawns.isBitSet(x, y))    stream << " P ";
                    if (board.blackRooks.isBitSet(x, y))    stream << " R ";
                    if (board.blackKnights.isBitSet(x, y))  stream << " N ";
                    if (board.blackBishops.isBitSet(x, y))  stream << " B ";
                    if (board.blackCar.isBitSet(x, y))      stream << " C ";
                }
            } else {
                setColor(stream, BRIGHT, WHITE, bgColor);
                stream << "   ";
            }

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

