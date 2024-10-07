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

//Test de vérification checkmate = false
TEST(TestBoard, isNotCheckmated) {
    Board* board = new Board();
    board->initializeBoard();
    board->movePiece(1,0,3,0,Color::WHITE);// pion blanc a2->a4
    board->movePiece(6,4,4,4,Color::BLACK);// pion noir e7->e5
    board->movePiece(1,6,3,6,Color::WHITE);// pion blanc g2->g4
    board->movePiece(6,0,4,0,Color::BLACK);// pion noir a7->a5
    bool roiBlancEchec = board->isCheckmated(0,4,Color::WHITE);
    EXPECT_FALSE(roiBlancEchec);
}

//Test de vérification d'un pat du côté des blancs
TEST(TestBoard, isPatWhite) {
    Board* board = new Board();
    board->initializeBoard();
    board->movePiece(1,4,2,4,Color::WHITE);// Pion blanc e2->e3
    board->movePiece(6,0,4,0,Color::BLACK);// Pion noir a7->a5
    board->movePiece(0,4,1,4,Color::WHITE);// Roi blanc e1->e2
    board->movePiece(6,7,4,7,Color::BLACK);// Pion  noir h7->h5
    board->movePiece(1,5,2,5,Color::WHITE);// Pion blanc f2->f3
    board->movePiece(4,0,3,0,Color::BLACK);// Pion noir a5->a4
    board->movePiece(1,4,1,5,Color::WHITE);// Roi blanc e2->f2
    board->movePiece(6,6,4,6,Color::BLACK);// Pion noir g7->g5
    board->movePiece(1,6,2,6,Color::WHITE);// Pion blanc g2->g3
    board->movePiece(3,0,2,0,Color::BLACK);// Pion noir a4->a3
    board->movePiece(1,5,1,6,Color::WHITE);// Roi blanc f2->g2
    board->movePiece(4,6,3,6,Color::BLACK);// Pion noir g5->g4
    board->movePiece(1,7,2,7,Color::WHITE);// Pion blanc h2->h3
    board->movePiece(2,0,1,0,Color::BLACK);// Pion noir a3->a2
    board->movePiece(1,6,1,7,Color::WHITE);// Roi blanc g2->h2
    board->movePiece(3,6,2,6,Color::BLACK);// Pion noir g4->g3
    board->movePiece(2,7,3,7,Color::WHITE);// Pion blanc h3->h4
    board->movePiece(4,7,3,7,Color::BLACK);// Pion noir h5->h4. Capture du pion blanc h4
    board->movePiece(1,7,2,7,Color::WHITE);// Roi blanc h2->h3. Préparation au Pat
    board->movePiece(2,6,1,6,Color::BLACK);// Pion noir g3->g2. Pat enclenché
    bool isPatWhite = board->isPat(2,7,Color::WHITE);
    EXPECT_TRUE(isPatWhite);
}

//Test de vérification d'un pat du côté des noirs
TEST(TestBoard, isPatBlack) {
    Board* board = new Board();
    board->initializeBoard();
    board->clearBoard();// Effacement du tableau pour placer librement des pièces
    board->placePiece(7,0, new Piece(TypePieces::KING, Color::BLACK));// Roi noir a8
    board->placePiece(5,1, new Piece(TypePieces::KING, Color::WHITE));// Roi blanc b6
    board->placePiece(6,2, new Piece(TypePieces::QUEEN, Color::WHITE));// Dame blanche c7
    bool isPatBlack = board->isPat(7,0,Color::BLACK);
    EXPECT_TRUE(isPatBlack);
}

//Test de vérification si il n y a pas de pat
TEST(TestBoard, notPat) {
    Board* board = new Board();
    board->initializeBoard();
    board->movePiece(1,4,3,4,Color::WHITE);// pion blanc e2->e4
    board->movePiece(6,4,4,4,Color::BLACK);// pion noir e7->e5
    board->movePiece(6,0,2,5,Color::WHITE);// cavalier blanc g1->f3. Capture du pion noir
    board->movePiece(6,3,5,3,Color::BLACK);// pion noir d7->d6
    bool isNotPat = board->isPat(7,4,Color::BLACK);
    EXPECT_FALSE(isNotPat);
}
