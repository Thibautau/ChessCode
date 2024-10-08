#include <gtest/gtest.h>

#include "Board.cpp"
// Test de l'initialisation du plateau
TEST(TestBoard, Initialization) {
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

TEST(TestBoard, PlacePiece) {
    Board* board = new Board();

    board->initializeBoard();


    Piece* newPiece = new Piece(TypePieces::QUEEN, Color::WHITE);
    board->placePiece(3, 3, newPiece);

    EXPECT_EQ(board->getPieceAt({3, 3}), newPiece);
    EXPECT_EQ(board->getPieceAt({3, 3})->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board->getPieceAt({3, 3})->getColor(), Color::WHITE);
}

// Test de la récupération d'une pièce
TEST(TestBoard, GetPieceAt) {
    Board* board = new Board();

    board->initializeBoard();


    Piece* piece = board->getPieceAt({0, 4}); // Roi blanc
    EXPECT_EQ(piece->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(piece->getColor(), Color::WHITE);
}

// Test du déplacement d'une pièce
TEST(TestBoard, MovePiece) {
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
TEST(TestBoard, IsMoveValid) {
    Board* board = new Board();

    board->initializeBoard();


    // Par exemple, vérifier que le mouvement est toujours valide (implémentation actuelle)
    EXPECT_TRUE(board->isMoveValid(1, 0, 2, 0));
}

//Test de vérification echec et mat pour roi noir
TEST(TestBoard, isBlackKingCheckmated) {
    Board* board = new Board();
    board->initializeBoard();
    board->movePiece(1,4,3,4,Color::WHITE);// pion blanc e2->e4
    board->movePiece(0,5,3,2,Color::WHITE);// fou blanc f1->c4
    board->movePiece(0,3,4,7,Color::WHITE);// dame blanche d1->h5
    board->movePiece(4,7,6,5,Color::WHITE);// dame blanche capture pion f7
    bool roiNoirEchec = board->isCheckmated(7,4,Color::BLACK);
    EXPECT_TRUE(roiNoirEchec);
}

//Test de vérification echec et mat pour roi blanc
TEST(TestBoard, isWhiteKingCheckmated) {
    Board* board = new Board();
    board->initializeBoard();
    board->movePiece(1,5,2,5,Color::WHITE);// pion blanc f2->f3
    board->movePiece(6,4,4,4,Color::BLACK);// pion noir e7->e5
    board->movePiece(1,6,3,6,Color::WHITE);// pion blanc g2->g4
    board->movePiece(7,3,3,7,Color::BLACK);// dame noire d8->h4
    bool roiBlancEchec = board->isCheckmated(0,4,Color::WHITE);
    EXPECT_TRUE(roiBlancEchec);
}