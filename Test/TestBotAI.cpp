//
// Created by Laurent on 27/11/2024.
//

#include "TestBotAI.h"

#include <gtest/gtest.h>
#include "Board.cpp"
#include "Bot.cpp"
#include "MainChessGame.cpp"
#include "PlayerHuman.cpp"
#include "Zobrist.cpp"
#include "LogFile.cpp"
#include "Piece.cpp"


class TestBotAI : public ::testing::Test {
protected:
    Board board;

    void SetUp() override {
        board.initializeBoard();
    }
};

// Test d'un crash qu'on a eu où le roi a le droit de bouger (TODO FONCTIONNE ?)
TEST_F(TestBotAI, KingCanMoveButGameCrashed)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("3k4/8/7B/2Q1R2P/3P4/4K3/P1P4P/8 w - - 21 13", &board);

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
    MainChessGame::setBoardFromFENStatic("4k3/1p1r3p/pB6/3pK3/4p1P1/3b4/7p/8 w - - 21 13", &board);

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
    MainChessGame::setBoardFromFENStatic("k7/8/8/3K4/8/8/7p/8 w - - 21 13", &board);

    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 1,cPromotion);
    EXPECT_EQ(iStart, 15);
    EXPECT_EQ(iEnd, 7);
    EXPECT_EQ(cPromotion, 'q'); // The queen seems to be the best choice
}

TEST_F(TestBotAI, TestBugAfterRock)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("r7/1p1k1N2/3b1np1/p2p1P1p/P7/3P1P2/P2B2PP/4K2R w K - 21 13", &board);

    bool result = board.movePiece("e1g1");

    EXPECT_TRUE(result);

    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 4,cPromotion);
    EXPECT_EQ(cPromotion, '\0');
}

TEST_F(TestBotAI, TestBugBeforeRock)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("4k2r/8/8/8/8/8/4P1P1/4RKR1 b k - 0 1", &board);

    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 4,cPromotion);
    EXPECT_EQ(iStart, 63);
    EXPECT_EQ(iEnd, 61);
    EXPECT_EQ(cPromotion, '\0');
}

TEST_F(TestBotAI, TestBugEnPassant)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("8/7k/8/8/pPp5/8/R6K/8 b - b3 0 1", &board);


    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 1,cPromotion);
    bool result = board.movePiece(iStart, iEnd, Color::BLACK);

    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt("a4"), nullptr);
    EXPECT_EQ(board.getPieceAt("b4"), nullptr);
    EXPECT_EQ(board.getPieceAt("b3")->getColor(), Color::BLACK);
    EXPECT_EQ(board.getPieceAt("b3")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(iStart, 24); // a4
    EXPECT_EQ(iEnd, 17); // b3
    EXPECT_EQ(cPromotion, '\0');
}

TEST_F(TestBotAI, TestEnPassant2)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("r1bqkb1r/pppp1ppp/2n1pn2/4P3/8/2N2N2/PPPP1PPP/R1BQKB1R b KQkq - 0 1", &board);


    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 2,cPromotion);
    bool result = board.movePiece(iStart, iEnd, Color::BLACK);

    EXPECT_TRUE(result);
}

TEST_F(TestBotAI, TestUndo)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("8/7k/8/8/pPp5/8/R6K/8 b - b3 0 1", &board);


    //Try to move the black pawn at h2 (promotion)
    Piece* capturedPiece = nullptr;
    int enPassantPos = board.getEnPassantPosition();
    char promoType = '\0';

    // Jouer le coup
    bool result = board.movePiece(24, 17, Color::BLACK, &capturedPiece, Piece::charToPieceType(promoType), &enPassantPos);

    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt("a4"), nullptr);
    EXPECT_EQ(board.getPieceAt("b4"), nullptr);
    EXPECT_EQ(board.getPieceAt("b3")->getColor(), Color::BLACK);
    EXPECT_EQ(board.getPieceAt("b3")->getTypePiece(), TypePieces::PAWN);

    //Undo le coup
    bool result2 = board.undoMove(24, 17, capturedPiece, false,enPassantPos);

    EXPECT_TRUE(result2);
    EXPECT_EQ(board.getPieceAt("a4")->getColor(), Color::BLACK);
    EXPECT_EQ(board.getPieceAt("b4")->getColor(), Color::WHITE);
    EXPECT_EQ(board.getPieceAt("a4")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("b4")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("b3"), nullptr);
}

TEST_F(TestBotAI, TestPourTrouverUnBug)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("k7/8/8/8/5PpP/8/8/K7 w - f3 0 1 ", &board);


    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 6,cPromotion);
    bool result = board.movePiece(iStart, iEnd, Color::BLACK);

    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt("g4"), nullptr);
    EXPECT_EQ(board.getPieceAt("f4"), nullptr);
    EXPECT_EQ(board.getPieceAt("f3")->getColor(), Color::BLACK);
    EXPECT_EQ(board.getPieceAt("f3")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(iStart, 30); // a4
    EXPECT_EQ(iEnd, 21); // b3
    EXPECT_EQ(cPromotion, '\0');
}

TEST_F(TestBotAI, TestPourTrouverUnBug2)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("k7/8/8/8/5PpP/8/8/K7 w - f3 0 1 ", &board);


    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 6,cPromotion);
    bool result = board.movePiece(iStart, iEnd, Color::BLACK);

    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt("g4"), nullptr);
    EXPECT_EQ(board.getPieceAt("f4"), nullptr);
    EXPECT_EQ(board.getPieceAt("f3")->getColor(), Color::BLACK);
    EXPECT_EQ(board.getPieceAt("f3")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(iStart, 30); // a4
    EXPECT_EQ(iEnd, 21); // b3
    EXPECT_EQ(cPromotion, '\0');
}

//TODO CRASH
TEST_F(TestBotAI, TestBug5)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("r1b1k2r/2p2p2/p1n2q1p/1pbpp2Q/8/1BNP2B1/PPP2PRP/R3K3 b Qkq - 0 1", &board);

    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 4,cPromotion);
    bool result = board.movePiece(iStart, iEnd, Color::BLACK);

    EXPECT_TRUE(result);
}

//TODO CRASH
TEST_F(TestBotAI, TestBug6)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("1r2k1nr/2p2ppp/p1nbb2q/1p1p4/1P2p2P/P1P1P1P1/3P1P2/RNBQKBNR w KQk - 0 1", &board);


    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 3,cPromotion);
    bool result = board.movePiece(iStart, iEnd, Color::BLACK);

    EXPECT_TRUE(result);
}

TEST_F(TestBotAI, TestBug7)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("1R3R2/6pp/8/k7/4q1PK/8/8/8 w - - 0 1", &board);

    bool result2 = board.movePiece("h4h5");
    EXPECT_TRUE(result2);

    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 2,cPromotion);
    bool result = board.movePiece(iStart, iEnd, Color::BLACK);

    EXPECT_TRUE(result);
}

TEST_F(TestBotAI, TestBug6Bis)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("1r2k1nr/2p2ppp/p1nbb2q/1p1p4/1P2p2P/P1P1P1P1/3P1P2/RNBQKBNR w KQk - 0 ", &board);

    //Try to move the black pawn at h2 (promotion)
    Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 3,cPromotion);
    bool result = board.movePiece(iStart, iEnd, Color::BLACK);

    EXPECT_TRUE(result);
}

TEST_F(TestBotAI, TestBug6Bis2)
{
    board.clearBoard();
    MainChessGame::setBoardFromFENStatic("1r2krn1/2p2ppp/p1nbb2q/1p1p4/1P2p2P/P1P1P1P1/3P1P2/RNBQKBNR b KQk - 0 ", &board);

    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(Color::BLACK);

    for (std::pair<int, int> pair : possibleMoves) {
        EXPECT_NE(pair, std::make_pair(60, 61));
    }

    //Try to move the black pawn at h2 (promotion)
    /*Bot* botBlack = new Bot(Color::BLACK);
    int iStart, iEnd = -1;
    char cPromotion = '\0';
    botBlack->playWithDepth(board, iStart, iEnd, 4,cPromotion);
    bool result = board.movePiece(iStart, iEnd, Color::BLACK);

    EXPECT_TRUE(result);*/
}
