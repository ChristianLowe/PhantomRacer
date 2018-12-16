#pragma once

#define TESTING false
#if TESTING

#include "move.h"
#include "board.h"

#define assertEQ(got, expect) {if((got)!=(expect)){std::cout<<"ERR: expected "<<(expect)<<", got "<<(got)<<" (line "<<__LINE__<<')';return false;}}

Board getEmptyBoard() {
    Board board;

    board.whitePawns     = 0;
    board.whiteKnights   = 0;
    board.whiteRooks     = 0;
    board.whiteBishops   = 0;
    board.whiteCar       = 0;

    board.blackPawns     = 0;
    board.blackKnights   = 0;
    board.blackRooks     = 0;
    board.blackBishops   = 0;
    board.blackCar       = 0;

    board.updatePieceAggregates();

    return board;
}

bool testPawn() {
    Board board = getEmptyBoard();
    board.whitePawns.flipBit(3, 3);
    board.blackPawns.flipBit(2, 1);
    board.blackCar.flipBit(4, 1);
    board.updatePieceAggregates();
    assertEQ(board.getValidMoves(PieceRange::White, false).size(), 1);

    board.performWhiteMove(board.getValidMoves(PieceRange::White, false)[0]);
    assertEQ(board.getValidMoves(PieceRange::White, false).size(), 2);

    board = getEmptyBoard();
    board.blackPawns.flipBit(3, 3);
    board.whitePawns.flipBit(2, 5);
    board.whiteCar.flipBit(4, 5);
    board.updatePieceAggregates();
    assertEQ(board.getValidMoves(PieceRange::Black, false).size(), 1);

    board.performBlackMove(board.getValidMoves(PieceRange::Black, false)[0]);
    assertEQ(board.getValidMoves(PieceRange::Black, false).size(), 2);

    return true;
}

bool testKnight() {
    Board board = getEmptyBoard();
    board.whiteKnights.flipBit(3, 3);  // +4 moves
    board.blackCar.flipBit(4, 1);      // -1 move
    board.blackPawns.flipBit(2, 1);    // no moves
    board.updatePieceAggregates();
    assertEQ(board.getValidMoves(PieceRange::White, false).size(), 3);

    board = getEmptyBoard();
    board.blackKnights.flipBit(3, 3);  // +4 moves
    board.whitePawns.flipBit(4, 1);    // +1 move
    board.whitePawns.flipBit(2, 1);    // +1 move
    board.whitePawns.flipBit(5, 2);    // +1 move
    board.whitePawns.flipBit(1, 2);    // +1 move
    board.updatePieceAggregates();
    assertEQ(board.getValidMoves(PieceRange::Black, false).size(), 8);

    return true;
}

bool testRook() {
    Board board = getEmptyBoard();
    board.whiteRooks.flipBit(2, 4);
    board.blackRooks.flipBit(2, 1);    // +3 moves
    board.blackBishops.flipBit(0, 4);  // +1 move
    board.blackPawns.flipBit(4, 4);    // +1 move
    board.blackPawns.flipBit(2, 7);    // +1 move
    board.updatePieceAggregates();
    assertEQ(board.getValidMoves(PieceRange::White, false).size(), 6);

    board = getEmptyBoard();
    board.whiteRooks.flipBit(2, 6);
    board.whitePawns.flipBit(2, 4);    // +1 move
    board.updatePieceAggregates();
    assertEQ(board.getValidMoves(PieceRange::White, false).size(), 2);

    board = getEmptyBoard();
    board.blackRooks.flipBit(1, 0);    // +7 moves
    board.whiteCar.flipBit(1, 6);      // -2 moves
    board.updatePieceAggregates();
    assertEQ(board.getValidMoves(PieceRange::Black, false).size(), 5);

    return true;
}

bool testBishop() {
    Board board = getEmptyBoard();
    board.whiteBishops.flipBit(3, 2);  // +4 moves
    board.blackPawns.flipBit(4, 1);    // -1 moves
    board.blackRooks.flipBit(1, 0);    // no moves
    board.blackKnights.flipBit(5, 4);  // +1 move
    board.updatePieceAggregates();
    assertEQ(board.getValidMoves(PieceRange::White, false).size(), 4);

    board = getEmptyBoard();
    board.blackBishops.flipBit(3, 5);  // +4 moves
    board.whitePawns.flipBit(4, 6);    // -1 move
    board.whiteRooks.flipBit(1, 7);    // no moves
    board.whiteKnights.flipBit(5, 3);  // +1 move
    board.updatePieceAggregates();
    assertEQ(board.getValidMoves(PieceRange::Black, false).size(), 4);

    return true;
}

bool testCar() {
    // Assert car can run over enemies if there's no other choice
    Board board = getEmptyBoard();
    board.blackCar.flipBit(0, 0);
    board.blackRooks.flipBit(2, 5);
    board.whitePawns.flipBit(1, 1);
    board.updatePieceAggregates();
    assertEQ(board.getValidMoves(PieceRange::Black).size(), 2);

    board.performBlackMove(board.getValidMoves(PieceRange::Black)[0]);
    assertEQ(board.getValidMoves(PieceRange::Black).size(), 1);

    board.performBlackMove(board.getValidMoves(PieceRange::Black)[0]);
    assertEQ(board.blackCar.isBitSet(0, 0), false);
    assertEQ(board.blackCar.isBitSet(1, 1), true);

    return true;
}

bool testRaycasting() {
    assertEQ(rayLookupTable[North][20], C64(0b1000000010000000100000001000000010000000000000000000000000000));
    assertEQ(rayLookupTable[NorthWest][20], C64(0b1000000100000010000001000000000000000000000000000));
    assertEQ(rayLookupTable[NorthEast][20], C64(0b100000000100000000000000000000000000000));
    assertEQ(rayLookupTable[East][20], C64(0b11000000000000000000000));

    assertEQ(rayLookupTable[South][44], C64(0b1000000010000000100000001000000010000));
    assertEQ(rayLookupTable[SouthWest][44], C64(0b100000000100000000100000000100000000));
    assertEQ(rayLookupTable[SouthEast][44], C64(0b10000001000000000000000000000000000000));
    assertEQ(rayLookupTable[West][44], C64(0b11110000000000000000000000000000000000000000));

    return true;
}

bool testLookup() {
    assertEQ(pieceLookupTable[4], C64(0b0000000000000000000000000000000000000000000000000000000000010000));
    assertEQ(maskPieceLookupTable[4], C64(0b1111111111111111111111111111111111111111111111111111111111101111));

    return true;
}

static int testCount = 0;

void test(const std::string& name, bool (*f)()) {
    testCount++;

    if (!f()) {
        std::cout <<" (in test: " << name << ")" << std::endl;
    }
}

void testingMain() {
    initAll();

    test("pawn", testPawn);
    test("knight", testKnight);
    test("rook", testRook);
    test("bishop", testBishop);
    test("car", testCar);

    test("raycasting", testRaycasting);
    test("lookup tables", testLookup);

    std::cout << "All " << testCount << " tests complete." << std::endl;
}

#endif

/*
    cout << board << endl;
    auto moves = board.getValidMoves(PieceRange::White);
    for (auto move : moves) {
        cout << move << ' ';
    }
    cout << endl;
 */