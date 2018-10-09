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
    EmptyPiece  = 00,

    BlackPawn   = 11,
    BlackBishop = 13,
    BlackRook   = 15,
    BlackKnight = 17,
    BlackCar    = 19,

    WhitePawn   = 21,
    WhiteBishop = 23,
    WhiteRook   = 25,
    WhiteKnight = 27,
    WhiteCar    = 29,
};
