#include <gtest/gtest.h>
#include "Board.h"

class BoardTest : public ::testing::Test {
protected:
    Board board;

    void SetUp() override {
        board.initializeBoard();
    }
};

TEST(PieceTest, PieceInitialisation) {
    Piece* piece = new Piece(TypePieces::PAWN, Color::WHITE);
    EXPECT_EQ(piece->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(piece->getColor(), Color::WHITE);
}

//!!!!!!!!!Test de déplacement des pieces valides!!!!!!!!!\\

//Deplacement Pion valide
TEST_F(BoardTest, ValidPawnMove) {
    bool result = board.movePiece(1, 4, 3, 4);//Pion blanc e2->e4
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(3, 4)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(1, 4), nullptr);
}

//Deplacement Pion Invalide
TEST_F(BoardTest, InvalidPawnMove) {
    bool result = board.movePiece(1, 4, 4, 4);//Pion blanc e2->e5
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(1, 4)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(4, 4), nullptr);
}

// Test de mouvement d'un pion après l'avoir déjà joué
TEST_F(BoardTest, PawnSecondMove) {
    bool firstMove = board.movePiece(1, 4, 3, 4);// Pion e2->e4
    EXPECT_TRUE(firstMove);
    EXPECT_EQ(board.getPieceAt(3, 4)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(1, 4), nullptr);

    bool secondMove = board.movePiece(3, 4, 5, 4);//Pion e4->e6
    EXPECT_FALSE(secondMove);
    EXPECT_EQ(board.getPieceAt(5, 4), nullptr);
    EXPECT_EQ(board.getPieceAt(3, 4)->getTypePiece(), TypePieces::PAWN);
}

// Test de capture en passant
TEST_F(BoardTest, EnPassantCapture) {
    board.movePiece(1, 4, 3, 4);// Pion blanc e2->e4
    board.movePiece(3, 4, 4, 4);// Pion blanc e4->e5
    board.movePiece(6, 5, 4, 5, Color::BLACK);// Pion noir f7->f5

    // Pion blanc de e5 capture en passant f6
    bool result = board.movePiece(4, 4, 5, 5);
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(5, 5)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(4, 5), nullptr); // Pion noir capturé
}

//Capture de Pion
TEST_F(BoardTest, CapturePion) {
    board.movePiece(1, 4, 3, 4); // Pion blanc e2 -> e4
    board.movePiece(6, 5, 4, 5, Color::BLACK); // Pion noir f7 -> f5
    //Capture du pion en f5
    bool result = board.movePiece(3, 4, 4, 5);
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(4, 5)->getColor(), Color::WHITE);
}

//Deplacer une piece dans une autre
TEST_F(BoardTest, MoveThroughPieces) {
    board.placePiece(2, 4, new Piece(TypePieces::PAWN, Color::WHITE)); // Pion blanc e3

    //Pion blanc e2->e4
    bool result = board.movePiece(1, 4, 3, 4);
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(1, 4)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(3, 4), nullptr);
}

// Test de castling valide (roi et tour n'ont pas bougé, pas d'échec)
TEST_F(BoardTest, ValidCastling) {
    // Déplacer les pions pour libérer le chemin
    board.movePiece(1, 4, 2, 4); // Pion blanc e2 -> e3
    board.movePiece(0, 5, 3, 2); // Bishop blanc f1 -> c4
    board.movePiece(0, 6, 2, 5); // Rook noir g7 -> f3

    //Castling roi côté droit (e1 -> g1)
    bool result = board.movePiece(0, 4, 0, 6);
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(0, 6)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(0, 5)->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt(0, 4), nullptr);
    EXPECT_EQ(board.getPieceAt(0, 7), nullptr);
}

// Test de castling invalide (roi en échec)
TEST_F(BoardTest, InvalidCastlingInCheck) {
    board.movePiece(1, 4, 3, 4); // Pion blanc e2 -> e4
    board.movePiece(0, 5, 3, 2); // Bishop blanc f1 -> c4
    board.movePiece(0, 6, 2, 5); // Rook noir g7 -> f3
    board.placePiece(7, 4, new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noire en e8

    // Essayer de castling roi côté roi (e1 -> g1) alors que le roi est en échec
    bool result = board.movePiece(0, 4, 0, 6);
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 4)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(0, 6), nullptr);
}

// Test impossibilité de castling après avoir bougé le roi
TEST_F(BoardTest, CastlingAfterKingMoved) {
    board.movePiece(1, 4, 3, 4);// Pion blanc e2 -> e4
    bool moveResult = board.movePiece(0, 4, 1, 4);// King blanc e1 -> e2
    EXPECT_TRUE(moveResult);
    EXPECT_EQ(board.getPieceAt(1, 4)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(0, 4), nullptr);

    // Essayer de castling roi côté roi (e1 -> g1) après avoir bougé le roi
    bool castlingResult = board.movePiece(0, 4, 0, 6);
    EXPECT_FALSE(castlingResult);
}

// Test impossibilité de castling après avoir bougé la tour
TEST_F(BoardTest, CastlingAfterRookMoved) {
    board.movePiece(1, 7, 3, 7);// Pion blanc h2 -> h4
    bool moveResult = board.movePiece(0, 7, 1, 7);//Rook blanc h1->h2
    EXPECT_TRUE(moveResult);
    EXPECT_EQ(board.getPieceAt(1, 7)->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt(0, 7), nullptr);

    // Essayer de castling roi côté roi (e1 -> g1) après avoir bougé la tour
    bool castlingResult = board.movePiece(0, 4, 0, 6);
    EXPECT_FALSE(castlingResult);

    board.movePiece(1, 0, 3, 0);// Pion blanc a2 -> a4
    bool moveResult2 = board.movePiece(0, 0, 1, 0);//Rook blanc a1->a2
    EXPECT_TRUE(moveResult2);
    EXPECT_EQ(board.getPieceAt(1, 0)->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt(0, 0), nullptr);

    // Essayer de castling roi côté roi (e1 -> c1) après avoir bougé la tour
    bool castlingResult2 = board.movePiece(0, 4, 0, 2);
    EXPECT_FALSE(castlingResult2);
}

// Test de mise en échec et mat
TEST_F(BoardTest, Checkmate) {
    // Placer le King blanc en d5
    board.placePiece(4, 3, new Piece(TypePieces::KING, Color::WHITE));
    // Placer la bishop blanche en e6
    board.placePiece(5, 4, new Piece(TypePieces::QUEEN, Color::WHITE));
    board.movePiece(6, 5, 4, 5); // Pion noir f7->f5
    bool validMove1 = board.movePiece(5, 4, 6, 5);// Bishop blanc e6->f7
    EXPECT_TRUE(validMove1);
}

// Test de mise en échec et mat
TEST_F(BoardTest, Checkmate2) {
    // Placer le King blanc en d5
    board.placePiece(4, 3, new Piece(TypePieces::KING, Color::WHITE));
    // Placer la bishop blanche en e6
    board.placePiece(5, 4, new Piece(TypePieces::QUEEN, Color::WHITE));
    board.movePiece(6, 5, 4, 5); // Pion noir f7->f5
    bool validMove2 = board.movePiece(5, 4, 7, 6);// Bishop blanc e6->g8
    EXPECT_TRUE(validMove2);
}

// Test de mise en échec et mat
TEST_F(BoardTest, Checkmate3) {
    // Placer le King blanc en d5
    board.placePiece(4, 3, new Piece(TypePieces::KING, Color::WHITE));
    // Placer la bishop blanche en e6
    board.placePiece(5, 4, new Piece(TypePieces::QUEEN, Color::WHITE));
    board.movePiece(6, 5, 4, 5); // Pion noir f7->f5
    bool invalidMove1 = board.movePiece(5, 4, 4, 5);// Bishop blanc e6->f5
    EXPECT_FALSE(invalidMove1);
}

// Test de mise en échec et mat
TEST_F(BoardTest, Checkmate4) {
    // Placer le King blanc en d5
    board.placePiece(4, 3, new Piece(TypePieces::KING, Color::WHITE));
    // Placer la bishop blanche en e6
    board.placePiece(5, 4, new Piece(TypePieces::QUEEN, Color::WHITE));
    board.movePiece(6, 5, 4, 5); // Pion noir f7->f5
    bool invalidMove2 = board.movePiece(5, 4, 6, 3);// Bishop blanc e6->d7
    EXPECT_FALSE(invalidMove2);
}

// Test de mouvement valide pour un cavalier (b1 -> c3)
TEST_F(BoardTest, ValidKnightMove) {
    bool result = board.movePiece(0, 1, 2, 2);
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 2)->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(board.getPieceAt(0, 1), nullptr);
}

//@TODO Marche pas
// Test de mouvement valide pour un cavalier (b1 -> a3)
TEST_F(BoardTest, ValidKnightMove2) {
    bool result = board.movePiece(0, 1, 2, 0);
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 0)->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(board.getPieceAt(0, 1), nullptr);
}

// Test de mouvement invalide pour un cavalier (b1 -> b3)
TEST_F(BoardTest, InvalidKnightMove) {
    bool result = board.movePiece(0, 1, 2, 1);
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 1)->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(board.getPieceAt(2, 1), nullptr);
}

// Test de mouvement valide pour une tour (a1 -> a3)
TEST_F(BoardTest, ValidRookMove) {
    board.movePiece(1, 0, 3, 0); // Pion blanc a3->a4
    bool result = board.movePiece(0, 0, 2, 0); // Rook blanc a1->a3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 0)->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt(0, 0), nullptr);

    bool result2 = board.movePiece(2, 0, 2, 1);// Rook blanc a3->b3
    EXPECT_TRUE(result2);
    EXPECT_EQ(board.getPieceAt(2, 1)->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt(2, 0), nullptr);
}

// Test de mouvement invalide pour une tour (a1->b2)
TEST_F(BoardTest, InvalidRookMove) {
    board.movePiece(1, 1, 3, 1);//Pion blanc b2->b4
    bool result = board.movePiece(0, 0, 1, 1); // Rook blanc a1->b2
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 0)->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt(1, 1), nullptr);
}

// Test de mouvement valide pour un fou (c1 -> e3)
TEST_F(BoardTest, ValidBishopMove) {
    board.movePiece(1, 3, 3, 3); // Pion blanc d2->d4
    bool result = board.movePiece(0, 2, 2, 4); // Bishop blanc c1->e3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 4)->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(board.getPieceAt(0, 2), nullptr);

    bool result2 = board.movePiece(2, 4, 4, 6); // Bishop blanc e3->g5
    EXPECT_TRUE(result2);
    EXPECT_EQ(board.getPieceAt(4, 6)->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(board.getPieceAt(2, 4), nullptr);
}

// Test de mouvement invalide pour un fou (c1 -> c2)
TEST_F(BoardTest, InvalidBishopMove) {
    board.movePiece(1, 2, 3, 2); // Pion blanc c2->c4
    bool result = board.movePiece(0, 2, 1, 2); // Bishop blanc c1->c2
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 2)->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(board.getPieceAt(1, 2), nullptr);
}


// Test de mouvement valide pour un cavalier (b1 -> c3)
TEST_F(BoardTest, ValidKnightMoveToC3) {
    bool result = board.movePiece(0, 1, 2, 2);// Knight b1->c3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 2)->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(board.getPieceAt(0, 1), nullptr);
}

// Test de mouvement invalide pour un cavalier (b1 -> b3)
TEST_F(BoardTest, InvalidKnightMoveToB3) {
    // Mouvement invalide car un cavalier ne peut pas se déplacer en ligne droite
    bool result = board.movePiece(0, 1, 2, 1);// Knight b1->b3
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 1)->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(board.getPieceAt(2, 1), nullptr);
}

// Test de mouvement valide pour un cavalier (b1 -> a3)
TEST_F(BoardTest, ValidKnightMoveToA3) {
    bool result = board.movePiece(0, 1, 2, 0);// Knight b1->a3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 0)->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(board.getPieceAt(0, 1), nullptr);
}

// Test de mouvement valide pour une Reine (d1 -> d4)
TEST_F(BoardTest, ValidQueenMoveToD4) {
    board.movePiece(1, 3, 3, 3); // Pion blanc d2 -> d4
    bool result = board.movePiece(0, 3, 2, 3); //Queen blanc d1->d3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 3)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(0, 3), nullptr);
}

// Test de mouvement valide pour une Reine (d1 -> d4)
TEST_F(BoardTest, ValidQueenMove2) {
    board.movePiece(1, 4, 2, 4); // Pion blanc e2 -> e3
    bool result = board.movePiece(0, 3, 4, 7); //Queen blanc d1->h5
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(4, 7)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(0, 3), nullptr);

    bool result2 = board.movePiece(4, 7, 4, 5); //Queen blanc h5->f5
    EXPECT_TRUE(result2);
    EXPECT_EQ(board.getPieceAt(4, 5)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(4, 7), nullptr);
}

// Test de mouvement valide pour une Reine (d1 -> h5)
TEST_F(BoardTest, ValidQueenDiagonalMove) {
    board.movePiece(1, 4, 2, 4); // Pion blanc e2 -> e3
    bool result = board.movePiece(0, 3, 4, 7);//Queen blanc d1->h5
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(4, 7)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(0, 3), nullptr);
}

// Test de mouvement invalide pour une Reine (d1 -> d5)
TEST_F(BoardTest, InvalidQueenMoveBlockedByPiece) {
    bool result = board.movePiece(0, 3, 4, 3); // Queen blanc d1->d5
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 3)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(4, 3), nullptr);
}

// Test de capture valide avec la Reine (d1 -> f3)
TEST_F(BoardTest, QueenCapturesPiece) {
    board.placePiece(2, 5, new Piece(TypePieces::PAWN, Color::BLACK));// Pion noir f3
    board.movePiece(1, 4, 2, 4); // Pion blanc e2 -> e3
    bool result = board.movePiece(0, 3, 2, 5);
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 5)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(0, 3), nullptr);
}

// Test de mouvement valide pour le roi (e1->e2)
TEST_F(BoardTest, ValidKingMove) {
    board.movePiece(1, 4, 3, 4);//Pion blanc e2->e4
    bool result = board.movePiece(0, 4, 1, 4); // Roi blanc e1->e2
    EXPECT_TRUE(result);

    //Piece* p = board.getPieceAt(0,4);
    EXPECT_EQ(board.getPieceAt(1, 4)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(0, 4), nullptr);

    bool b = true;
}

// Test de mouvement valide pour le roi (e1->d2)
TEST_F(BoardTest, ValidKingMove2) {
    board.movePiece(1, 3, 3, 3);//Pion blanc d2->d4
    bool result = board.movePiece(0, 4, 1, 3); // Roi blanc e1->d2
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(1, 3)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(0, 4), nullptr);
}

// Test de mouvement invalide pour le roi (e1->e3)
TEST_F(BoardTest, invalidKingMove) {
    board.movePiece(1, 4, 3, 4);//Pion blanc e2->e4
    bool result = board.movePiece(0, 4, 2, 4); // Roi blanc e1->e3
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 4)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(2, 4), nullptr);
}

//Le roi en échec s'il bouge (e3->f4)
TEST_F(BoardTest, invalidKingMove2) {
    board.movePiece(1, 4, 3, 4);//Pion blanc e2->e4
    board.movePiece(6, 4, 4, 4, Color::BLACK);//Pion noir e2->e4
    board.movePiece(0, 4, 1, 4); // Roi blanc e1->e2
    board.movePiece(1, 4, 2, 4); // Roi blanc e2->e3
    bool result = board.movePiece(2, 4, 3, 5); // Roi blanc e3->f4
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(2, 4)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(3, 5), nullptr);
}