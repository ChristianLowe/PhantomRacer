#pragma once

#define TESTING false
#if TESTING

#include "move.h"
#include "board.h"

#define assertEQ(got, expect) {if((got)!=(expect)){std::cout<<"ERR: expected "<<(expect)<<", got "<<(got);return false;}}

Board getEmptyBoard() {
    Board board;
    for (u8 y = 0; y < 8; y++) {
        for (u8 x = 0; x < 7; x++) {
            board.setPiece(x, y, 00);
        }
    }
    return board;
}

bool testRook() {
    Board board = getEmptyBoard();
    board.setPiece(2, 4, HumanRook);
    board.setPiece(2, 1, CpuAiRook);    // +3 moves
    board.setPiece(0, 4, CpuAiBishop);  // +1 move
    board.setPiece(4, 4, CpuAiPawn);    // +1 move
    board.setPiece(2, 7, CpuAiPawn);    // +1 move
    assertEQ(board.getValidMoves(PieceRange::Human).size(), 6);

    board = getEmptyBoard();
    board.setPiece(1, 0, CpuAiRook);    // +7 moves
    board.setPiece(1, 6, HumanCar);     // -2 moves
    assertEQ(board.getValidMoves(PieceRange::CpuAi).size(), 5);

    return true;
}

bool testBishop() {
    Board board = getEmptyBoard();
    board.setPiece(3, 2, HumanBishop);  // +4 moves
    board.setPiece(4, 1, CpuAiPawn);    // -1 moves
    board.setPiece(1, 0, CpuAiRook);    // no moves
    board.setPiece(5, 4, CpuAiKnight);  // +1 move
    assertEQ(board.getValidMoves(PieceRange::Human).size(), 4);

    board = getEmptyBoard();
    board.setPiece(3, 5, CpuAiBishop);  // +4 moves
    board.setPiece(4, 6, HumanPawn);    // -1 move
    board.setPiece(1, 7, HumanRook);    // no moves
    board.setPiece(5, 3, HumanKnight);  // +1 move
    assertEQ(board.getValidMoves(PieceRange::CpuAi).size(), 4);

    return true;
}

bool testKnight() {
    Board board = getEmptyBoard();
    board.setPiece(3, 3, HumanKnight);  // +4 moves
    board.setPiece(4, 1, CpuAiCar);     // -1 move
    board.setPiece(2, 1, CpuAiPawn);    // no moves
    assertEQ(board.getValidMoves(PieceRange::Human).size(), 3);

    board = getEmptyBoard();
    board.setPiece(3, 3, CpuAiKnight);  // +4 moves
    board.setPiece(4, 1, HumanPawn);    // +1 move
    board.setPiece(2, 1, HumanPawn);    // +1 move
    board.setPiece(5, 2, HumanPawn);    // +1 move
    board.setPiece(1, 2, HumanPawn);    // +1 move
    assertEQ(board.getValidMoves(PieceRange::CpuAi).size(), 8);

    return true;
}

bool testPawn() {
    Board board = getEmptyBoard();
    board.setPiece(3, 3, HumanPawn);
    board.setPiece(2, 1, CpuAiPawn);
    board.setPiece(4, 1, CpuAiCar);
    assertEQ(board.getValidMoves(PieceRange::Human).size(), 1);

    board.performMove(board.getValidMoves(PieceRange::Human)[0]);
    assertEQ(board.getValidMoves(PieceRange::Human).size(), 2);

    board = getEmptyBoard();
    board.setPiece(3, 3, CpuAiPawn);
    board.setPiece(2, 5, HumanPawn);
    board.setPiece(4, 5, HumanCar);
    assertEQ(board.getValidMoves(PieceRange::CpuAi).size(), 1);

    board.performMove(board.getValidMoves(PieceRange::CpuAi)[0]);
    assertEQ(board.getValidMoves(PieceRange::CpuAi).size(), 2);

    return true;
}

bool testCar() {
    // Assert car can run over enemies if there's no other choice
    Board board = getEmptyBoard();
    board.setPiece(0, 0, CpuAiCar);
    board.setPiece(2, 5, CpuAiRook);
    board.setPiece(1, 1, HumanPawn);
    assertEQ(board.getValidMoves(PieceRange::CpuAi).size(), 2);

    board.performMove(board.getValidMoves(PieceRange::CpuAi)[1]);
    assertEQ(board.getValidMoves(PieceRange::CpuAi).size(), 1);

    board.performMove(board.getValidMoves(PieceRange::CpuAi)[0]);
    assertEQ(board.getPiece(1, 1), CpuAiCar);

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
    test("rook", testRook);
    test("bishop", testBishop);
    test("knight", testKnight);
    test("pawn", testPawn);
    test("car", testCar);

    std::cout << "All " << testCount << " tests complete." << std::endl;
}

#endif
