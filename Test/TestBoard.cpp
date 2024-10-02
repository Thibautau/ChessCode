#include <gtest/gtest.h>

#include "Board.cpp"
// Test de l'initialisation du plateau
TEST(BoardTest, Initialization) {
    Board* board = new Board();

    board->initializeBoard();


    // Vérifie que la case (0,0) contient une tour blanche
    EXPECT_NE(board->getPieceAt({0, 0}), nullptr);
    EXPECT_EQ(board->getPieceAt({0, 0})->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board->getPieceAt({0, 0})->getColor(), Color::WHITE);

    // Vérifie que la case (7,4) contient un roi noir
    EXPECT_NE(board->getPieceAt({7, 4}), nullptr);
    EXPECT_EQ(board->getPieceAt({7, 4})->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board->getPieceAt({7, 4})->getColor(), Color::BLACK);
}

TEST(BoardTest, PlacePiece) {
    Board* board = new Board();

    board->initializeBoard();


    Piece* newPiece = new Piece(TypePieces::QUEEN, Color::WHITE);
    board->placePiece(3, 3, newPiece);

    EXPECT_EQ(board->getPieceAt({3, 3}), newPiece);
    EXPECT_EQ(board->getPieceAt({3, 3})->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board->getPieceAt({3, 3})->getColor(), Color::WHITE);
}

// Test de la récupération d'une pièce
TEST(BoardTest, GetPieceAt) {
    Board* board = new Board();

    board->initializeBoard();


    Piece* piece = board->getPieceAt({0, 4}); // Roi blanc
    EXPECT_EQ(piece->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(piece->getColor(), Color::WHITE);
}

// Test du déplacement d'une pièce
TEST(BoardTest, MovePiece) {
    Board* board = new Board();

    board->initializeBoard();


    // Déplace un pion blanc de (1, 0) à (2, 0)
    bool moved = board->movePiece(1, 0, 2, 0);
    EXPECT_TRUE(moved);
    EXPECT_NE(board->getPieceAt({2, 0}), nullptr);
    EXPECT_EQ(board->getPieceAt({2, 0})->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board->getPieceAt({1, 0}), nullptr);
}

// Test de la validation d'un mouvement (actuellement toujours vrai)
TEST(BoardTest, IsMoveValid) {
    Board* board = new Board();

    board->initializeBoard();


    // Par exemple, vérifier que le mouvement est toujours valide (implémentation actuelle)
    EXPECT_TRUE(board->isMoveValid(1, 0, 2, 0));
}
