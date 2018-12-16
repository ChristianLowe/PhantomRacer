#pragma once

#include <cstdint>

#define C64(x) x##ULL

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

typedef uint_fast8_t u8;
typedef uint_fast16_t u16;
typedef uint_fast32_t u32;
typedef uint_fast64_t u64;

const u64 U64_MAX = C64(0xFFFFFFFFFFFFFFFF);

enum PieceType : u8 {
    EmptyPiece  = 0,

    BlackPawn   = 1,
    BlackKnight = 2,
    BlackRook   = 3,
    BlackBishop = 4,
    BlackCar    = 5,

    WhitePawn   = 6,
    WhiteKnight = 7,
    WhiteRook   = 8,
    WhiteBishop = 9,
    WhiteCar    = 10,
};
