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

// Test d'un crash qu'on a eu où le roi a le droit de bouger
TEST_F(TestBotAI, GameCrashDueToPawnPromotion)
{
    board.clearBoard();
    board.setupFromFEN("4k3/1p1r3p/pB6/3pK3/4p1P1/3b4/7p/8 w - - 21 13");

    bool result = board.movePiece("e5f5");
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt("e5"), nullptr);
    EXPECT_EQ(board.getPieceAt("f5")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("f5")->getColor(), Color::WHITE);

    bool isBlackKingCheck = board.isBlackKingCheck();
    bool isWhiteKingCheck = board.isWhiteKingCheck();
    Color colWinner = Color::NONE;
    bool isGameOverForBlack = board.isGameOver(Color::BLACK, colWinner);
    bool isGameOverForWhite = board.isGameOver(Color::WHITE, colWinner);

    EXPECT_FALSE(isBlackKingCheck);
    EXPECT_FALSE(isWhiteKingCheck);
    EXPECT_FALSE(isGameOverForBlack);
    EXPECT_FALSE(isGameOverForWhite);
    EXPECT_EQ(Color::NONE, colWinner);


    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 2,cPromotion);
    EXPECT_EQ(iStart, 15);
    EXPECT_EQ(iEnd, 7);
    EXPECT_EQ(cPromotion, 'q'); // The queen seems to be the best choice


    isBlackKingCheck = board.isBlackKingCheck();
    isWhiteKingCheck = board.isWhiteKingCheck();
    colWinner = Color::NONE;
    isGameOverForWhite = board.isGameOver(Color::WHITE, colWinner);
    isGameOverForBlack = board.isGameOver(Color::BLACK, colWinner);
    EXPECT_FALSE(isBlackKingCheck);
    EXPECT_FALSE(isWhiteKingCheck);
    EXPECT_FALSE(isGameOverForWhite);
    EXPECT_FALSE(isGameOverForBlack);
    EXPECT_EQ(Color::NONE, colWinner);
}

// Test d'un crash qu'on a eu où le roi a le droit de bouger
TEST_F(TestBotAI, GameCrashDueToPawnPromotion2)
{
    board.clearBoard();
    board.setupFromFEN("8/8/8/3K4/8/8/7p/8 w - - 21 13");

    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 2,cPromotion);
    EXPECT_EQ(iStart, 15);
    EXPECT_EQ(iEnd, 7);
    EXPECT_EQ(cPromotion, 'q'); // The queen seems to be the best choice
}

TEST_F(TestBotAI, TestBugAfterRock)
{
    board.clearBoard();
    board.setupFromFEN("r7/1p1k1N2/3b1np1/p2p1P1p/P7/3P1P2/P2B2PP/4K2R w - - 21 13");

    bool result = board.movePiece("e1g1");

    EXPECT_TRUE(result);

    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 6,cPromotion);
    EXPECT_EQ(cPromotion, '\0');
}