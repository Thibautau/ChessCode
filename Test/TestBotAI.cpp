//
// Created by Laurent on 27/11/2024.
//

#include "TestBotAI.h"

#include <gtest/gtest.h>
#include "Board.h"
#include "Bot.h"


class TestBotAI : public ::testing::Test {
protected:
    Board board;

    void SetUp() override {
        board.initializeBoard();
    }
};

// Test d'un crash qu'on a eu où le roi a le droit de bouger
TEST_F(TestBotAI, KingCanMoveButGameCrashed)
{
    board.clearBoard();
    board.setupFromFEN("3k4/8/7B/2Q1R2P/3P4/4K3/P1P4P/8 w - - 21 13");

    bool result = board.movePiece("h6g5");
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt("h6"), nullptr);
    EXPECT_EQ(board.getPieceAt("g5")->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(board.getPieceAt("g5")->getColor(), Color::WHITE);

    bool isCheck = board.isBlackKingCheck();
    Color colWinner = Color::NONE;
    bool isGameOver = board.isGameOver(Color::BLACK, colWinner);

    EXPECT_TRUE(isCheck);
    EXPECT_FALSE(isGameOver);
    EXPECT_EQ(Color::NONE, colWinner);

    std::vector<std::pair<int, int>> listOfMoves = board.listOfPossibleMoves(Color::BLACK);
    EXPECT_EQ(listOfMoves.size(), 1);
    EXPECT_EQ(listOfMoves[0].first, 59);
    EXPECT_EQ(listOfMoves[0].second, 51);

    bool isWhiteKingChecked = board.isWhiteKingCheck();
    colWinner = Color::NONE;
    bool isGameOverForWhite = board.isGameOver(Color::WHITE, colWinner);
    EXPECT_FALSE(isWhiteKingChecked);
    EXPECT_FALSE(isGameOverForWhite);


    //Try to move the black king at d7 (below)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion;
    botBlack->playWithDepth(board, iStart, iEnd, 6,cPromotion);
    EXPECT_EQ(iStart, 59);
    EXPECT_EQ(iEnd, 51);

    isCheck = board.isBlackKingCheck();
    colWinner = Color::NONE;
    isGameOver = board.isGameOver(Color::WHITE, colWinner);
    EXPECT_FALSE(isCheck);
    EXPECT_FALSE(isGameOver);
    EXPECT_EQ(Color::NONE, colWinner);
}

