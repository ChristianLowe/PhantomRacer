#pragma once

#include "types.h"

const u64 rightColMask = C64(0x7F7F7F7F7F7F7F7F);
const u64 leftColMask  = C64(0xFEFEFEFEFEFEFEFE);
const u64 rightTwoColMask = rightColMask & C64(0xBFBFBFBFBFBFBFBF);
const u64 leftTwoColMask  = leftColMask  & C64(0xFDFDFDFDFDFDFDFD);

enum ScanDirection {
    // Positive
    North       = 0,  // +8
    NorthWest   = 1,  // +7
    NorthEast   = 2,  // +9
    East        = 3,  // +1

    // Negative
    South       = 4,  // -8
    SouthWest   = 5,  // -9
    SouthEast   = 6,  // -7
    West        = 7,  // -1
};

u64 pieceLookupTable[64];
u64 maskPieceLookupTable[64];
u64 knightLookupTable[64];
u64 rayLookupTable[8][64];

class BitBoard {
public:
    u64 bits;

    BitBoard(u64 newBits = 0) : bits(newBits) {}

public:
    void flipBit(int x, int y) {
        bits ^= static_cast<u64>(1) << static_cast<u8>(((7 - y) * 8) + x);
    }

    bool isBitSet(int x, int y) const {
        return (bits & pieceLookupTable[((7 - y) * 8) + x]) != 0;
    }
};

void initPieceLookupTable() {
    for (u64 i = 0; i < 64; i++) {
        u64 piece = static_cast<u64>(1) << i;
        pieceLookupTable[i] = piece;
        maskPieceLookupTable[i] = ~piece;
    }
}

void initKnightLookupTable() {
    for (u64 i = 0; i < 64; i++) {
        knightLookupTable[i] = 0;
        u64 knight = static_cast<u64>(1) << i;
        u64 spot[8];

        spot[0] = (knight & leftTwoColMask)  << 6u;
        spot[1] = (knight & leftColMask)     << 15u;
        spot[2] = (knight & rightColMask)    << 17u;
        spot[3] = (knight & rightTwoColMask) << 10u;

        spot[4] = (knight & rightTwoColMask) >> 6u;
        spot[5] = (knight & rightColMask)    >> 15u;
        spot[6] = (knight & leftColMask)     >> 17u;
        spot[7] = (knight & leftTwoColMask)  >> 10u;

        for (u64 s : spot) {
            knightLookupTable[i] |= s;
        }

        knightLookupTable[i] &= rightColMask;
    }
}

void initRayLookupTable() {
    int directionMap[8];

    // Right Shift
    directionMap[North]     = 8;
    directionMap[NorthWest] = 7;
    directionMap[NorthEast] = 9;
    directionMap[East]      = 1;

    // Left Shift
    directionMap[South]     = 8;
    directionMap[SouthWest] = 9;
    directionMap[SouthEast] = 7;
    directionMap[West]      = 1;

    // North
    for (u32 j = 0; j < 64; j++) {
        u64 attackRay = 0;
        u32 currentBitPosition = j + directionMap[North];
        while (currentBitPosition < 64) {
            attackRay |= static_cast<u64>(1) << currentBitPosition;
            currentBitPosition += directionMap[North];
        }
        rayLookupTable[North][j] = attackRay & rightColMask;
    }

    // North-west
    for (u32 j = 0; j < 64; j++) {
        u64 attackRay = 0;
        u32 currentBitPosition = j + directionMap[NorthWest];
        while (currentBitPosition < 64 && currentBitPosition % 8 != 7) {
            attackRay |= static_cast<u64>(1) << currentBitPosition;
            currentBitPosition += directionMap[NorthWest];
        }
        rayLookupTable[NorthWest][j] = attackRay & rightColMask;
    }

    // North-east
    for (u32 j = 0; j < 64; j++) {
        u64 attackRay = 0;
        u32 currentBitPosition = j + directionMap[NorthEast];
        while (currentBitPosition < 64 && currentBitPosition % 8 != 0) {
            attackRay |= static_cast<u64>(1) << currentBitPosition;
            currentBitPosition += directionMap[NorthEast];
        }
        rayLookupTable[NorthEast][j] = attackRay & rightColMask;
    }

    // East
    for (u32 j = 0; j < 64; j++) {
        u64 attackRay = 0;
        u32 currentBitPosition = j + directionMap[East];
        while (currentBitPosition < 64 && currentBitPosition % 8 != 0) {
            attackRay |= static_cast<u64>(1) << currentBitPosition;
            currentBitPosition += directionMap[East];
        }
        rayLookupTable[East][j] = attackRay & rightColMask;
    }

    // South
    for (u32 j = 0; j < 64; j++) {
        u64 attackRay = 0;
        u32 currentBitPosition = j - directionMap[South];
        while (currentBitPosition < 64) {
            attackRay |= static_cast<u64>(1) << currentBitPosition;
            currentBitPosition -= directionMap[South];
        }
        rayLookupTable[South][j] = attackRay & rightColMask;
    }

    // South-west
    for (u32 j = 0; j < 64; j++) {
        u64 attackRay = 0;
        u32 currentBitPosition = j - directionMap[SouthWest];
        while (currentBitPosition < 64 && currentBitPosition % 8 != 7) {
            attackRay |= static_cast<u64>(1) << currentBitPosition;
            currentBitPosition -= directionMap[SouthWest];
        }
        rayLookupTable[SouthWest][j] = attackRay & rightColMask;
    }

    // South-east
    for (u32 j = 0; j < 64; j++) {
        u64 attackRay = 0;
        u32 currentBitPosition = j - directionMap[SouthEast];
        while (currentBitPosition < 64 && currentBitPosition % 8 != 0) {
            attackRay |= static_cast<u64>(1) << currentBitPosition;
            currentBitPosition -= directionMap[SouthEast];
        }
        rayLookupTable[SouthEast][j] = attackRay & rightColMask;
    }

    // West
    for (u32 j = 0; j < 64; j++) {
        u64 attackRay = 0;
        u32 currentBitPosition = j - directionMap[West];
        while (currentBitPosition < 64 && currentBitPosition % 8 != 7) {
            attackRay |= static_cast<u64>(1) << currentBitPosition;
            currentBitPosition -= directionMap[West];
        }
        rayLookupTable[West][j] = attackRay & rightColMask;
    }
}
