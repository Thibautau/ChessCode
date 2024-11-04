#include <gtest/gtest.h>
#include "UCI/UCI.cpp"
#include "MainChessGame.cpp"
#include "Board.h"
#include "Bot.cpp"

class MainChessGameTest : public ::testing::Test {
protected:
    MainChessGame* game;

    void SetUp() override {
        game = new MainChessGame(GameMode::JVJ);
    }
};


// Test pour la position de départ
TEST_F(MainChessGameTest, SetBoardFromFEN_StartPosition) {
    std::string startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    game->setBoardFromFEN(startFEN);

    EXPECT_EQ(game->getPieceAt("a1")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(game->getPieceAt("a1")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("e1")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(game->getPieceAt("e1")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("e8")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(game->getPieceAt("e8")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("a8")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(game->getPieceAt("a8")->getColor(), Color::BLACK);

    EXPECT_EQ(game->getCurrentPlayer()->getPlayerColor(), Color::WHITE);
    EXPECT_TRUE(game->getBoard()->kingCanBigRock(Color::WHITE));
    EXPECT_TRUE(game->getBoard()->kingCanLittleRock(Color::WHITE));
    EXPECT_TRUE(game->getBoard()->kingCanBigRock(Color::BLACK));
    EXPECT_TRUE(game->getBoard()->kingCanLittleRock(Color::BLACK));
    EXPECT_EQ(game->getBoard()->getEnPassantPosition(), -1);
}

// Test pour une position en milieu de partie
TEST_F(MainChessGameTest, SetBoardFromFEN_MidgamePosition) {
    std::string midFEN = "r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R b KQkq - 4 3";
    game->setBoardFromFEN(midFEN);

    //BLACK
    EXPECT_EQ(game->getPieceAt("b8"), nullptr);
    EXPECT_EQ(game->getPieceAt("g8"), nullptr);
    EXPECT_EQ(game->getPieceAt("e7"), nullptr);
    EXPECT_EQ(game->getPieceAt("a8")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(game->getPieceAt("a8")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("c8")->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(game->getPieceAt("c8")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("d8")->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(game->getPieceAt("d8")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("e8")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(game->getPieceAt("e8")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("f8")->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(game->getPieceAt("f8")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("h8")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(game->getPieceAt("h8")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("c6")->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(game->getPieceAt("c6")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("f6")->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(game->getPieceAt("f6")->getColor(), Color::BLACK);

    EXPECT_EQ(game->getPieceAt("a7")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("a7")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("b7")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("b7")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("c7")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("c7")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("d7")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("d7")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("f7")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("f7")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("g7")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("g7")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("h7")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("h7")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("e5")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("e5")->getColor(), Color::BLACK);

    //WHITE
    EXPECT_EQ(game->getPieceAt("b1"), nullptr);
    EXPECT_EQ(game->getPieceAt("g1"), nullptr);
    EXPECT_EQ(game->getPieceAt("e2"), nullptr);
    EXPECT_EQ(game->getPieceAt("a1")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(game->getPieceAt("a1")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("c1")->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(game->getPieceAt("c1")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("d1")->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(game->getPieceAt("d1")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("e1")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(game->getPieceAt("e1")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("f1")->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(game->getPieceAt("f1")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("h1")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(game->getPieceAt("h1")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("c3")->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(game->getPieceAt("c3")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("f3")->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(game->getPieceAt("f3")->getColor(), Color::WHITE);

    EXPECT_EQ(game->getPieceAt("a2")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("a2")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("b2")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("b2")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("c2")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("c2")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("d2")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("d2")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("f2")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("f2")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("g2")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("g2")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("h2")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("h2")->getColor(), Color::WHITE);
    EXPECT_EQ(game->getPieceAt("e4")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("e4")->getColor(), Color::WHITE);


    EXPECT_EQ(game->getCurrentPlayer()->getPlayerColor(), Color::BLACK);
    EXPECT_TRUE(game->getBoard()->kingCanBigRock(Color::WHITE));
    EXPECT_TRUE(game->getBoard()->kingCanLittleRock(Color::WHITE));
    EXPECT_TRUE(game->getBoard()->kingCanBigRock(Color::BLACK));
    EXPECT_TRUE(game->getBoard()->kingCanLittleRock(Color::BLACK));
    EXPECT_EQ(game->getBoard()->getEnPassantPosition(), -1);
    EXPECT_FALSE(game->getBoard()->isKingInCheck(Color::WHITE));
    EXPECT_FALSE(game->getBoard()->isKingInCheck(Color::BLACK));
}

// Test pour vérifier la position de prise en passant
TEST_F(MainChessGameTest, SetBoardFromFEN_EnPassantPosition) {
    std::string enPassantFEN = "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2";
    game->setBoardFromFEN(enPassantFEN);

    EXPECT_EQ(game->getBoard()->getEnPassantPosition(), 44);
}

// Test pour les droits de roque dans une position particulière
TEST_F(MainChessGameTest, SetBoardFromFEN_CastlingRights) {
    std::string castlingFEN = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";
    game->setBoardFromFEN(castlingFEN);

    EXPECT_TRUE(game->getBoard()->kingCanBigRock(Color::WHITE));
    EXPECT_TRUE(game->getBoard()->kingCanLittleRock(Color::WHITE));
    EXPECT_TRUE(game->getBoard()->kingCanBigRock(Color::BLACK));
    EXPECT_TRUE(game->getBoard()->kingCanLittleRock(Color::BLACK));
    EXPECT_EQ(game->getPieceAt("a1")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(game->getPieceAt("h1")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(game->getPieceAt("a8")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(game->getPieceAt("h8")->getTypePiece(), TypePieces::ROOK);
}

// Test pour une position sans roque et sans prise en passant
TEST_F(MainChessGameTest, SetBoardFromFEN_NoCastlingNoEnPassant) {
    std::string noCastlingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";
    game->setBoardFromFEN(noCastlingFEN);

    EXPECT_FALSE(game->getBoard()->kingCanBigRock(Color::WHITE));
    EXPECT_FALSE(game->getBoard()->kingCanLittleRock(Color::WHITE));
    EXPECT_FALSE(game->getBoard()->kingCanBigRock(Color::BLACK));
    EXPECT_FALSE(game->getBoard()->kingCanLittleRock(Color::BLACK));
    EXPECT_EQ(game->getBoard()->getEnPassantPosition(), -1);
}

// Test pour une position avec une configuration avancée de pions et de pièces
TEST_F(MainChessGameTest, SetBoardFromFEN_AdvancedPosition) {
    std::string advancedFEN = "8/2p5/3P4/8/8/8/8/8 w - - 0 1";
    game->setBoardFromFEN(advancedFEN);

    EXPECT_EQ(game->getPieceAt("c7")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("c7")->getColor(), Color::BLACK);
    EXPECT_EQ(game->getPieceAt("d6")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(game->getPieceAt("d6")->getColor(), Color::WHITE);
}

TEST_F(MainChessGameTest, SetBoardFromFEN_WhiteKingInCheck) {
    // FEN où le roi blanc en e1 est en échec par la reine noire en e5
    std::string checkFEN = "4k3/8/8/4q3/8/8/8/4K3 w - - 0 1";
    game->setBoardFromFEN(checkFEN);

    EXPECT_TRUE(game->getBoard()->isKingInCheck(Color::WHITE));
    EXPECT_FALSE(game->getBoard()->isKingInCheck(Color::BLACK));
}