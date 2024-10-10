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

//@TODO Ne marche pas
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
    //Pion blanc e2->e4
    bool result = board.movePiece("e1e2");
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 4)->getTypePiece(), TypePieces::KING);
}

//@TODO Ne marche pas
// Test de castling valide (roi et tour n'ont pas bougé, pas d'échec)
TEST_F(BoardTest, ValidCastling) {
    // Déplacer les pions pour libérer le chemin
    board.movePiece(1, 4, 2, 4); // Pion blanc e2 -> e3
    board.movePiece(0, 5, 3, 2); // Bishop blanc f1 -> c4
    board.movePiece(0, 6, 2, 5); // Knight blanc  g1 -> f3

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
    board.movePiece(0, 6, 2, 5); // Knight blanc  g1 -> f3
    board.placePiece(4, 4, new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noire en e5
    board.movePiece(4, 4, 3, 4, Color::BLACK); // Tour noir  e5->e4, check

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

//@TODO Ne marche pas
// Test impossibilité de castling après avoir bougé la tour
TEST_F(BoardTest, CastlingAfterRookMoved) {
    board.movePiece(1, 7, 3, 7); // Pion blanc h2 -> h4
    bool moveResult = board.movePiece(0, 7, 1, 7); //Rook blanc h1->h2
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

//@TODO Ne marche pas
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

//@TODO Ne marche pas
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

//@TODO Ne marche pas
//Le roi en échec s'il bouge (b4->c4)
TEST_F(BoardTest, invalidKingMove3) {
    board.placePiece(3,0, new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noir en a4
    board.placePiece(3,1, new Piece(TypePieces::KING, Color::WHITE)); // Roi blanc en b4
    bool result = board.movePiece(3, 1, 3, 2); // Roi blanc b4->c4 (impossible car encore en échec)
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(3, 1)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(3, 2), nullptr);
}

//@TODO Ne marche pas
//Le roi en échec si le fou bouge (b4->c5)
TEST_F(BoardTest, kingInCheck) {
    board.clearBoard();

    board.placePiece(3,0, new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noir en a4
    board.placePiece(3,1, new Piece(TypePieces::BISHOP, Color::BLACK)); // Fou noir en b4
    board.placePiece(3,2, new Piece(TypePieces::KING, Color::WHITE)); // Roi blanc en c4
    bool result = board.movePiece(3, 1, 4, 2, Color::BLACK); // Fou noir b4->c5
    bool result2 = board.isWhiteKingCheck();
    EXPECT_TRUE(result);
    EXPECT_TRUE(result2);
    EXPECT_EQ(board.getPieceAt(4, 2)->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(board.getPieceAt(3, 1), nullptr);
}

//Le roi en échec le fou bouge (b4->c5)
TEST_F(BoardTest, invalidKingInCheck) {
    board.clearBoard();

    board.placePiece(3,0, new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noir en a4
    board.placePiece(3,1, new Piece(TypePieces::BISHOP, Color::WHITE)); // Fou blanc en b4
    board.placePiece(3,2, new Piece(TypePieces::KING, Color::WHITE)); // Roi blanc en c4
    bool result = board.movePiece(3, 1, 4, 2); // Fou blanc b4->c5
    bool result2 = board.isWhiteKingCheck();
    EXPECT_FALSE(result);
    EXPECT_FALSE(result2);
    EXPECT_EQ(board.getPieceAt(3, 1)->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(board.getPieceAt(4, 2), nullptr);
}

//@TODO Ne marche pas
TEST_F(BoardTest, PromotePawnToQueen) {
    std::istringstream input("1\n");
    std::cin.rdbuf(input.rdbuf());

    Piece* pPiece = new Piece(TypePieces::PAWN, Color::WHITE);
    board.placePiece(6, 0, pPiece);


    bool result = board.movePiece(6, 0, 7, 1, Color::WHITE);

    EXPECT_TRUE(result);

    Piece* promotedPiece = board.getPieceAt(7, 1);
    EXPECT_EQ(promotedPiece->getTypePiece(), TypePieces::QUEEN);

    // Nettoyez si nécessaire
    delete pPiece;
    delete promotedPiece;
}


///////////////////////////////////////////////////////////////


// Test avancée d'une case en avant
TEST_F(BoardTest, PawnAdvanceOne) {
    bool result = board.movePiece(1, 4, 2, 4); // Pion blanc e2->e3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 4)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(1, 4), nullptr);
}

// Test avancée de deux cases depuis la position initiale
TEST_F(BoardTest, PawnAdvanceTwoFromInitial) {
    bool result = board.movePiece(1, 4, 3, 4); // Pion blanc e2->e4
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(3, 4)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(1, 4), nullptr);
}

// Test avancée de deux cases non depuis la position initiale (doit échouer)
TEST_F(BoardTest, PawnAdvanceTwoNotFromInitial) {
    board.movePiece(1, 4, 2, 4); // Pion blanc e2->e3
    bool result = board.movePiece(2, 4, 4, 4); // Pion blanc e3->e5
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(2, 4)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(4, 4), nullptr);
}

// Test capture en diagonale
TEST_F(BoardTest, PawnCaptureDiagonal) {
    board.placePiece(2, 5, new Piece(TypePieces::PAWN, Color::BLACK)); // Pion noir en f3
    bool result = board.movePiece(1, 4, 2, 5); // Pion blanc e2->f3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 5)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(2, 5)->getColor(), Color::WHITE);
    EXPECT_EQ(board.getPieceAt(1, 4), nullptr);
}

// Test capture en passant
TEST_F(BoardTest, PawnEnPassantCapture) {
    board.movePiece(1, 4, 3, 4); // Pion blanc e2->e4
    board.movePiece(6, 5, 4, 5, Color::BLACK); // Pion noir f7->f5
    bool result = board.movePiece(3, 4, 4, 5); // Pion blanc e4->f5 en passant
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(4, 5)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(4, 5)->getColor(), Color::WHITE);
    EXPECT_EQ(board.getPieceAt(3, 4), nullptr);
}

//@TODO Ne marche pas
// Test promotion du pion
TEST_F(BoardTest, PawnPromotion) {
    board.placePiece(6, 0, new Piece(TypePieces::PAWN, Color::WHITE));
    // Simuler la promotion en dame
    bool result = board.movePiece(6, 0, 7, 0); // Pion blanc promotion
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(7, 0)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(7, 0)->getColor(), Color::WHITE);
}

// Test mouvement invalide (pion reculant)
TEST_F(BoardTest, PawnInvalidMoveBackward) {
    board.movePiece(1, 4, 2, 4); // Pion blanc e2->e3
    bool result = board.movePiece(2, 4, 1, 4); // Pion blanc e3->e2 (recul)
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(2, 4)->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt(1, 4), nullptr);
}

// Test mouvement invalide (pion avançant sur une pièce)
TEST_F(BoardTest, PawnInvalidMoveForwardBlocked) {
    board.placePiece(2, 4, new Piece(TypePieces::PAWN, Color::WHITE)); // Bloquer le pion
    bool result = board.movePiece(1, 4, 2, 4); // Pion blanc e2->e3
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(1, 4)->getTypePiece(), TypePieces::PAWN);
}

// Test capture invalide (pion capturant en avant)
TEST_F(BoardTest, PawnInvalidCaptureForward) {
    board.placePiece(2, 4, new Piece(TypePieces::PAWN, Color::BLACK)); // Pion noir en e3
    bool result = board.movePiece(1, 4, 2, 4); // Pion blanc e2->e3
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(1, 4)->getTypePiece(), TypePieces::PAWN);
}

//
// Tests pour la Tour (Rook)
//

// Test déplacement horizontal
TEST_F(BoardTest, RookValidHorizontalMove) {
    board.movePiece(1, 0, 3, 0); // Pion blanc a2->a4
    bool result = board.movePiece(0, 0, 2, 0); // Tour blanche a1->a3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 0)->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt(0, 0), nullptr);
}

// Test déplacement vertical
TEST_F(BoardTest, RookValidVerticalMove) {
    board.placePiece(4, 4, new Piece(TypePieces::ROOK, Color::WHITE)); // Tour noir en e5
    bool result = board.movePiece(4, 4, 4, 5); // Tour blanc e5->f5
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(4, 5)->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt(4, 4), nullptr);
}

// Test mouvement invalide (tour se déplaçant en diagonale)
TEST_F(BoardTest, RookInvalidDiagonalMove) {
    bool result = board.movePiece(0, 0, 1, 1); // Tour blanche a1->b2
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 0)->getTypePiece(), TypePieces::ROOK);
}

// Test mouvement bloqué par une pièce alliée
TEST_F(BoardTest, RookBlockedByAlly) {
    bool result = board.movePiece(0, 0, 1, 0); // Tour blanche a1->a2
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 0)->getTypePiece(), TypePieces::ROOK);
}

//
// Tests pour le Cavalier (Knight)
//

// Test mouvement en 'L'
TEST_F(BoardTest, KnightValidMoveLShape) {
    bool result = board.movePiece(0, 1, 2, 2); // Cavalier blanc b1->c3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 2)->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(board.getPieceAt(0, 1), nullptr);
}

// Test capture par le cavalier
TEST_F(BoardTest, KnightCapture) {
    board.placePiece(2, 2, new Piece(TypePieces::PAWN, Color::BLACK)); // Pion noir en c3
    bool result = board.movePiece(0, 1, 2, 2); // Cavalier blanc b1->c3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(2, 2)->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(board.getPieceAt(2, 2)->getColor(), Color::WHITE);
}

// Test mouvement invalide (cavalier se déplaçant en ligne droite)
TEST_F(BoardTest, KnightInvalidStraightMove) {
    bool result = board.movePiece(0, 1, 2, 1); // Cavalier blanc b1->b3
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 1)->getTypePiece(), TypePieces::KNIGHT);
    EXPECT_EQ(board.getPieceAt(2, 1), nullptr);
}

//
// Tests pour le Fou (Bishop)
//

// Test déplacement en diagonale
TEST_F(BoardTest, BishopValidDiagonalMove) {
    board.movePiece(1, 3, 2, 3); // Pion blanc c2->c3
    bool result = board.movePiece(0, 2, 3, 5); // Fou blanc c1->f4
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(3, 5)->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(board.getPieceAt(0, 2), nullptr);
}

// Test capture par le fou
TEST_F(BoardTest, BishopCapture) {
    board.movePiece(1, 3, 2, 3); // Pion blanc d2->d3
    board.placePiece(3, 5, new Piece(TypePieces::KNIGHT, Color::BLACK)); // Cavalier noir en f4
    bool result = board.movePiece(0, 2, 3, 5); // Fou blanc c1->f4
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(3, 5)->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(board.getPieceAt(3, 5)->getColor(), Color::WHITE);
}

// Test mouvement invalide (fou se déplaçant horizontalement)
TEST_F(BoardTest, BishopInvalidHorizontalMove) {
    bool result = board.movePiece(0, 2, 0, 5); // Fou blanc c1->f1
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 2)->getTypePiece(), TypePieces::BISHOP);
}

//
// Tests pour la Dame (Queen)
//

//@TODO Verifier
// Test déplacement en ligne droite
TEST_F(BoardTest, QueenValidStraightMove) {
    board.movePiece(1, 3, 2, 3); // Pion blanc d2->d3
    bool result = board.movePiece(0, 3, 3, 3); // Dame blanche d1->d4
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(3, 3)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(0, 3), nullptr);
}

// Test déplacement en diagonale
TEST_F(BoardTest, QueenValidDiagonalMove) {
    board.movePiece(1, 4, 2, 4); // Pion blanc e2->e3
    bool result = board.movePiece(0, 3, 4, 7); // Dame blanche d1->h5
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(4, 7)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(0, 3), nullptr);
}

// Test mouvement invalide (dame se déplaçant en 'L')
TEST_F(BoardTest, QueenInvalidKnightMove) {
    bool result = board.movePiece(0, 3, 2, 2); // Dame blanche d1->c3
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 3)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(2, 2), nullptr);
}

//
// Tests pour le Roi (King)
//

// Test déplacement d'une case dans toutes les directions
TEST_F(BoardTest, KingValidMoves) {
    board.placePiece(4, 4, new Piece(TypePieces::KING, Color::WHITE));
    bool result1 = board.movePiece(4, 4, 3, 4); // Roi blanc e5->e4
    EXPECT_TRUE(result1);
    bool result2 = board.movePiece(3, 4, 3, 3); // Roi blanc e4->d4
    EXPECT_TRUE(result2);
    bool result3 = board.movePiece(3, 3, 4, 3); // Roi blanc d4->d5
    EXPECT_TRUE(result3);
    bool result4 = board.movePiece(4, 3, 4, 4); // Roi blanc d5->e5
    EXPECT_TRUE(result4);
}

//@TODO Ne marche pas
// Test roque côté roi (petit roque)
TEST_F(BoardTest, KingCastlingKingside) {
    // Libérer le chemin
    board.movePiece(1, 5, 2, 5); // Pion blanc f2->f3
    board.movePiece(1, 6, 2, 6); // Pion blanc g2->g3
    board.movePiece(0, 5, 1, 4); // Fou blanc f1->e2
    board.movePiece(0, 6, 2, 5); // Cavalier blanc g1->f3
    // Effectuer le roque
    bool result = board.movePiece(0, 4, 0, 6); // Roi blanc e1->g1
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(0, 6)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(0, 5)->getTypePiece(), TypePieces::ROOK);
}

// Test roque invalide (roi en échec)
TEST_F(BoardTest, KingInvalidCastlingWhileInCheck) {
    // Libérer le chemin
    board.movePiece(1, 5, 2, 5); // Pion blanc f2->f3
    board.movePiece(1, 6, 2, 6); // Pion blanc g2->g3
    board.movePiece(0, 5, 1, 4); // Fou blanc f1->e2
    board.movePiece(0, 6, 2, 5); // Cavalier blanc g1->f3
    // Placer une tour noire attaquant le roi
    board.placePiece(4, 4, new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noire en e5
    // Essayer de roquer
    bool result = board.movePiece(0, 4, 0, 6); // Roi blanc e1->g1
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 4)->getTypePiece(), TypePieces::KING);
}

// Test échec au roi
TEST_F(BoardTest, KingInCheck) {
    board.placePiece(4, 4, new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noire en e5
    bool result = board.movePiece(0, 4, 1, 4); // Roi blanc e1->e2
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(0, 4)->getTypePiece(), TypePieces::KING);
}


