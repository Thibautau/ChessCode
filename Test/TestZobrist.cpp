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


TEST_F(TestZobrist, TestHashDifferentForSamePositionButNotSamePersonToPlay2)
{
    Zobrist::initZobrist();

    board.clearBoard();
    board.setupFromFEN("7k/8/8/8/8/8/8/K7 w - - 21 13");
    board.setZobristHash(Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), board.getEnPassantState()));
    uint64_t initBoardHash = board.getZobristHash();

    bool boardMoveResult = board.movePiece("a1b2");


    board.setZobristHash(Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), board.getEnPassantState()));
    uint64_t intermediaireBoardHash = board.getZobristHash();

    EXPECT_TRUE(boardMoveResult);

    uint64_t endBoardHashForMove = intermediaireBoardHash;
    Bot::calculateZobristHashForMove(board, {9,0}, Color::WHITE, '\0', false, endBoardHashForMove);

    board.movePiece("b2a1");

    EXPECT_EQ(endBoardHashForMove, Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), board.getEnPassantState()));
    EXPECT_EQ(endBoardHashForMove, initBoardHash);
}

TEST_F(TestZobrist, initialisation)
{
    Zobrist::initZobrist();
    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    uint64_t hash2 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    EXPECT_EQ(hash1, hash2);
}

TEST_F(TestZobrist, movement)
{
    Zobrist::initZobrist();
    board.movePiece("b2b3");
    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    uint64_t hash2 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    EXPECT_EQ(hash1, hash2);
}

TEST_F(TestZobrist, movement2)
{
    Zobrist::initZobrist();
    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    uint64_t hash2 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    Bot::calculateZobristHashForMove(board, {9,17}, Color::WHITE, '\0', false, hash1);
    Bot::calculateZobristHashForMove(board, {9,17}, Color::WHITE, '\0', false, hash2);
    EXPECT_EQ(hash1, hash2);
}

TEST_F(TestZobrist, TwoBoard)
{
    Zobrist::initZobrist();
    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    uint64_t hash2 = Zobrist::computeZobristHash(board2.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    EXPECT_EQ(hash1, hash2);
}

TEST_F(TestZobrist, TwoBoardTestCalculating)
{
    Zobrist::initZobrist();
    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    uint64_t hash2 = Zobrist::computeZobristHash(board2.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    Bot::calculateZobristHashForMove(board, {9,17}, Color::WHITE, '\0', false, hash1);
    Bot::calculateZobristHashForMove(board, {9,17}, Color::WHITE, '\0', false, hash2);
    EXPECT_EQ(hash1, hash2);
}

TEST_F(TestZobrist, TwoBoardTestCalculating2)
{
    Zobrist::initZobrist();
    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);
    uint64_t hash2 = Zobrist::computeZobristHash(board2.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    uint64_t hash1_base = hash1;
    uint64_t hash2_base = hash2;

    Bot::calculateZobristHashForMove(board, {9,17}, Color::WHITE, '\0', false, hash1);
    Bot::calculateZobristHashForMove(board, {9,17}, Color::WHITE, '\0', false, hash2);

    board.movePiece("b2b3");
    board2.movePiece("b2b3");

    Bot::calculateZobristHashForMove(board, {17,9}, Color::WHITE, '\0', false, hash1);
    Bot::calculateZobristHashForMove(board, {17,9}, Color::WHITE, '\0', false, hash2);
    EXPECT_EQ(hash1, hash1_base);
    EXPECT_EQ(hash2, hash2_base);
    EXPECT_EQ(hash2, hash1);
}

TEST_F(TestZobrist, TestHashDifferentForSamePositionButNotSamePersonToPlay3) {
    Zobrist::initZobrist();

    board.clearBoard();
    board.setupFromFEN("7k/8/8/8/8/8/8/K7 w - - 21 13");
    uint64_t hash1_base = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    Bot::calculateZobristHashForMove(board, {0,9}, Color::WHITE, '\0', false, hash1);
    board.movePiece("a1b2");
    Bot::calculateZobristHashForMove(board, {9,0}, Color::WHITE, '\0', false, hash1);

    EXPECT_EQ(hash1, hash1_base);
}

TEST_F(TestZobrist, TestHashDifferentForSamePositionButNotSamePersonToPlay4) {
    Zobrist::initZobrist();

    board.clearBoard();
    board.setupFromFEN("6kq/8/8/8/8/8/8/QK6 w - - 21 13");
    uint64_t hash1_base = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    Bot::calculateZobristHashForMove(board, {0,9}, Color::WHITE, '\0', false, hash1);
    board.movePiece("a1b2");
    Bot::calculateZobristHashForMove(board, {9,0}, Color::WHITE, '\0', false, hash1);

    EXPECT_EQ(hash1, hash1_base);
}


TEST_F(TestZobrist, TestHashDifferentForSamePositionButNotSamePersonToPlay5) {
    Zobrist::initZobrist();

    board.clearBoard();
    board.setupFromFEN("6kq/8/8/8/8/8/8/QK6 w - - 21 13");
    uint64_t hash1_base = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    Bot::calculateZobristHashForMove(board, {1,2}, Color::WHITE, '\0', false, hash1);
    board.movePiece("b1c1");
    Bot::calculateZobristHashForMove(board, {2,1}, Color::WHITE, '\0', false, hash1);

    EXPECT_EQ(hash1, hash1_base);
}

TEST_F(TestZobrist, TestHashDifferentForSamePositionButNotSamePersonToPlay6) {
    Zobrist::initZobrist();

    board.clearBoard();
    board.setupFromFEN("6kq/8/8/8/8/8/8/QK6 w - - 21 13");
    uint64_t hash1_base = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    board.movePiece("a1b2");
    board.movePiece("b2a1");

    uint64_t hash2 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    EXPECT_EQ(hash1, hash2);
    EXPECT_EQ(hash1, hash1_base);
}

TEST_F(TestZobrist, Capture) {
    Zobrist::initZobrist();

    board.clearBoard();
    board.setupFromFEN("6kq/8/8/8/8/7Q/8/K7 w - - 21 13");
    uint64_t hash1_base = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    uint64_t hash1 = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), -1);

    Bot::calculateZobristHashForMove(board, {63,23}, Color::WHITE, '\0', false, hash1);
    board.movePiece("h8h3");
    Bot::calculateZobristHashForMove(board, {23,63}, Color::WHITE, '\0', false, hash1);

    EXPECT_EQ(hash1, hash1_base);
}