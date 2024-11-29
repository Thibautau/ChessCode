//
// Created by Laurent on 29/11/2024.
//

#include "TestZobrist.h"
#include <gtest/gtest.h>
#include "Board.h"
#include "Bot.h"
#include "Zobrist.h"

class TestZobrist : public ::testing::Test {
protected:
    Board board;
    Board board2;

    void SetUp() override {
        board.initializeBoard();
        board2.initializeBoard();
    }
};

// Test if the hash is different if the user to play is white or black for a same position
TEST_F(TestZobrist, TestHashDifferentForSamePositionButNotSamePersonToPlay)
{
    Zobrist::initZobrist();

    board.clearBoard();
    board2.clearBoard();
    board.setupFromFEN("7k/8/8/8/8/8/8/K7 w - - 21 13");
    board2.setupFromFEN("7k/8/8/8/8/8/8/K7 b - - 21 13");
    board.setZobristHash(Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), board.getEnPassantState()));
    board2.setZobristHash(Zobrist::computeZobristHash(board2.getBoardStateAsVector(), true, board2.getCastlingStateAsVector(), board2.getEnPassantState()));


    uint64_t initBoardHash = board.getZobristHash();
    uint64_t initBoard2Hash = board2.getZobristHash();

    EXPECT_NE(initBoardHash, initBoard2Hash);


    bool boardMoveResult = board.movePiece("a1b2");
    bool boardMoveResult2 = board.movePiece("h8h7", Color::BLACK);
    bool board2MoveResult = board2.movePiece("h8h7", Color::BLACK);
    bool board2MoveResult2 = board2.movePiece("a1b2");

    board.setZobristHash(Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), board.getEnPassantState()));
    board2.setZobristHash(Zobrist::computeZobristHash(board2.getBoardStateAsVector(), true, board2.getCastlingStateAsVector(), board2.getEnPassantState()));
    uint64_t intermediaireBoardHash = board.getZobristHash();
    uint64_t intermediaireBoard2Hash = board2.getZobristHash();

    EXPECT_TRUE(boardMoveResult);
    EXPECT_TRUE(boardMoveResult2);
    EXPECT_TRUE(board2MoveResult);
    EXPECT_TRUE(board2MoveResult2);
    EXPECT_NE(intermediaireBoardHash, intermediaireBoard2Hash);

    uint64_t endBoardHashForMove = intermediaireBoardHash;
    uint64_t endBoard2HashForMove = intermediaireBoard2Hash;
    Bot::calculateZobristHashForMove(board, {9,0}, Color::WHITE, '\0', false, endBoardHashForMove);
    Bot::calculateZobristHashForMove(board2, {55, 63}, Color::BLACK, '\0', false, endBoard2HashForMove);

    EXPECT_NE(endBoardHashForMove, endBoard2HashForMove);
    EXPECT_EQ(endBoardHashForMove, initBoardHash);
    EXPECT_EQ(endBoard2HashForMove, initBoard2Hash);
}