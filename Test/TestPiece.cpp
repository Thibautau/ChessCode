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
    board.movePiece("e2e4");// Pion blanc e2->e4
    board.movePiece("e4e5");// Pion blanc e4->e5
    board.movePiece("f7f5", Color::BLACK);// Pion noir f7->f5

    // Pion blanc de e5 capture en passant f6
    bool result = board.movePiece("e5f6");
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
    board.movePiece("e2e4"); // Pion blanc e2 -> e4
    board.movePiece("f1c4"); // Bishop blanc f1 -> c4
    board.movePiece("g1f3"); // Knight blanc  g1 -> f3
    board.placePiece("e5", new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noire en e5
    board.movePiece("e5e4", Color::BLACK); // Tour noir  e5->e4, check

    // Essayer de castling roi côté roi (e1 -> g1) alors que le roi est en échec
    bool result = board.movePiece("e1g1");
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

//Le roi en échec s'il bouge (e3->f4)
TEST_F(BoardTest, invalidKingMove2) {
    board.movePiece("e2e4");//Pion blanc e2->e4
    board.movePiece("e7e5", Color::BLACK);//Pion noir e2->e4
    board.movePiece("e1e2"); // Roi blanc e1->e2
    board.movePiece("e2e3"); // Roi blanc e2->e3
    bool result = board.movePiece(2, 4, 3, 5); // Roi blanc e3->f4
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(2, 4)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(3, 5), nullptr);
}


//Le roi en échec s'il bouge (b4->c4)
TEST_F(BoardTest, invalidKingMove3) {
    board.placePiece(3,0, new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noir en a4
    board.placePiece(3,1, new Piece(TypePieces::KING, Color::WHITE)); // Roi blanc en b4
    bool result = board.movePiece(3, 1, 3, 2); // Roi blanc b4->c4 (impossible car encore en échec)
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt(3, 1)->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt(3, 2), nullptr);
}


//Le roi en échec si le fou bouge (b4->c5)
TEST_F(BoardTest, kingInCheck) {
    board.clearBoard();

    board.placePiece("a4", new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noir en a4
    board.placePiece("b4", new Piece(TypePieces::BISHOP, Color::BLACK)); // Fou blanc en b4
    board.placePiece("c4", new Piece(TypePieces::KING, Color::WHITE)); // Roi blanc en c4
    bool result = board.movePiece("b4c5", Color::BLACK); // Fou noir b4->c5
    bool result2 = board.isWhiteKingCheck();
    EXPECT_TRUE(result);
    EXPECT_TRUE(result2);
    EXPECT_EQ(board.getPieceAt("c5")->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(board.getPieceAt("b4"), nullptr);
}

//Le roi en échec le fou bouge (b4->c5)
TEST_F(BoardTest, invalidKingInCheck) {
    board.clearBoard();

    board.placePiece("a4", new Piece(TypePieces::ROOK, Color::BLACK)); // Tour noir en a4
    board.placePiece("b4", new Piece(TypePieces::BISHOP, Color::WHITE)); // Fou blanc en b4
    board.placePiece("c4", new Piece(TypePieces::KING, Color::WHITE)); // Roi blanc en c4
    bool result = board.movePiece("b4c5"); // Fou blanc b4->c5
    bool result2 = board.isWhiteKingCheck();
    EXPECT_FALSE(result);
    EXPECT_FALSE(result2);
    EXPECT_EQ(board.getPieceAt("b4")->getTypePiece(), TypePieces::BISHOP);
    EXPECT_EQ(board.getPieceAt("c5"), nullptr);
}


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


// Test promotion du pion
TEST_F(BoardTest, PawnPromotion) {
    board.placePiece("a7", new Piece(TypePieces::PAWN, Color::WHITE));
    //bool result = board.movePiece("a7b8"); // Pion blanc promotion
    bool result = board.movePiece(48,57,Color::WHITE,nullptr, TypePieces::QUEEN); // Pion blanc promotion a7 -> b7
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt(7, 1)->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt(7, 1)->getColor(), Color::WHITE);
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

// Test déplacement en ligne droite
TEST_F(BoardTest, QueenValidStraightMove) {
    board.movePiece("d2d4"); // Pion blanc d2->d4
    bool result = board.movePiece("d1d3"); // Dame blanche d1->d3
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt("d3")->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt("d1"), nullptr);
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
    //board.clearBoard();
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

// Test roque côté roi (petit roque)
TEST_F(BoardTest, KingCastlingKingside) {
    // Libère le chemin pour le grand roque noir
    board.setupFromFEN("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");

    // Essai de roquer
    bool result = board.movePiece("e8g8", Color::BLACK);
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt("g8")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("g8")->getColor(), Color::BLACK);
    EXPECT_EQ(board.getPieceAt("f8")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt("f8")->getColor(), Color::BLACK);

    bool result2 = board.movePiece("e1g1");
    EXPECT_TRUE(result2);
    EXPECT_EQ(board.getPieceAt("g1")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("g1")->getColor(), Color::WHITE);
    EXPECT_EQ(board.getPieceAt("f1")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt("f1")->getColor(), Color::WHITE);
}

// Test grand roque invalide après mouvement d'une tour
TEST_F(BoardTest, KingInvalidBigCastlingDueToRookMoving) {
    // Libère le chemin
    board.movePiece("d2d3");
    board.movePiece("c1e3");
    board.movePiece("d1d2");
    board.movePiece("b1a3");
    board.movePiece("a1b1");
    // Essai de roquer
    bool result = board.movePiece("e1c1");
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt("e1")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("b1")->getTypePiece(), TypePieces::ROOK);
}

// Test petit roque invalide après mouvement d'une tour
TEST_F(BoardTest, KingInvalidLittleCastlingDueToRookMoving) {
    // Libère le chemin
    board.movePiece("e2e3");
    board.movePiece("f1d3");
    board.movePiece("g1h3");
    board.movePiece("h1g1");
    board.movePiece("g1h1");
    // Essai de roquer
    bool result = board.movePiece("e1g1");
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt("e1")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("h1")->getTypePiece(), TypePieces::ROOK);
}

// Test grand roque invalide après mouvement du roi
TEST_F(BoardTest, KingInvalidBigCastlingDueToKingMoving) {
    // Libère le chemin
    board.movePiece("d2d3");
    board.movePiece("c1e3");
    board.movePiece("d1d2");
    board.movePiece("b1a3");
    board.movePiece("e1d1");
    board.movePiece("d1e1");
    // Essai de roquer
    bool result = board.movePiece("e1c1");
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt("e1")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("a1")->getTypePiece(), TypePieces::ROOK);
}

// Test petit roque invalide après mouvement du roi
TEST_F(BoardTest, KingInvalidLittleCastlingDueToKingMoving) {
    // Libère le chemin
    board.movePiece("e2e3");
    board.movePiece("f1d3");
    board.movePiece("g1h3");
    board.movePiece("e1f1");
    board.movePiece("f1e1");
    // Essai de roquer
    bool result = board.movePiece("e1g1");
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt("e1")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("h1")->getTypePiece(), TypePieces::ROOK);
}

// Test petit roque invalide après mouvement du roi
TEST_F(BoardTest, KingBigCastling) {
    // Libère le chemin pour le grand roque blanc
    board.setupFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KBNR w KQkq - 0 1");

    // Essai de roquer
    bool result = board.movePiece("e1c1");
    EXPECT_TRUE(result);
    EXPECT_EQ(board.getPieceAt("c1")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("c1")->getColor(), Color::WHITE);
    EXPECT_EQ(board.getPieceAt("d1")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt("d1")->getColor(), Color::WHITE);


    board.clearBoard();
    // Libère le chemin pour le grand roque noir
    board.setupFromFEN("r3kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // Essai de roquer
    bool result2 = board.movePiece("e8c8", Color::BLACK);
    EXPECT_TRUE(result2);
    EXPECT_EQ(board.getPieceAt("c8")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("c8")->getColor(), Color::BLACK);
    EXPECT_EQ(board.getPieceAt("d8")->getTypePiece(), TypePieces::ROOK);
    EXPECT_EQ(board.getPieceAt("d8")->getColor(), Color::BLACK);
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


// ----------------------------------- UndoMove -----------------------------------

// Test déplacement normal
TEST_F(BoardTest, UndoNormalMove) {
    board.placePiece("e2", new Piece(TypePieces::PAWN, Color::WHITE));
    board.movePiece("e2e4", Color::WHITE);
    EXPECT_EQ(board.getPieceAt("e4")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("e2"), nullptr);

    board.undoMove(12,28 ,nullptr, false);
    EXPECT_EQ(board.getPieceAt("e2")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("e4"), nullptr);
}

// Test d'une capture
TEST_F(BoardTest, UndoCaptureMove) {
    board.placePiece("d4", new Piece(TypePieces::PAWN, Color::WHITE));
    board.placePiece("e5", new Piece(TypePieces::PAWN, Color::BLACK));
    board.movePiece("d4e5", Color::WHITE);
    EXPECT_EQ(board.getPieceAt("e5")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("d4"), nullptr);

    board.undoMove(27, 36, new Piece(TypePieces::PAWN, Color::BLACK), false);
    EXPECT_EQ(board.getPieceAt("d4")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("e5")->getTypePiece(), TypePieces::PAWN);
}

// Test d'une promotion avec undo
TEST_F(BoardTest, UndoPromotionMove) {
    board.placePiece("a7", new Piece(TypePieces::PAWN, Color::WHITE));
    board.movePiece(48,57, Color::WHITE, nullptr,TypePieces::QUEEN); //a7->b8
    EXPECT_EQ(board.getPieceAt("b8")->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt("a7"), nullptr);

    board.undoMove(48, 57, new Piece(TypePieces::KNIGHT, Color::WHITE), true);
    EXPECT_EQ(board.getPieceAt("a7")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("b8")->getTypePiece(), TypePieces::KNIGHT);
}

// Test d'une prise en passant avec undo
TEST_F(BoardTest, UndoEnPassantMove) {
    board.placePiece("e5", new Piece(TypePieces::PAWN, Color::WHITE));
    board.movePiece("f7f5", Color::BLACK);
    board.movePiece("e5f6", Color::WHITE);
    EXPECT_EQ(board.getPieceAt("f6")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("e5"), nullptr);
    EXPECT_EQ(board.getPieceAt("f5"), nullptr);

    board.undoMove(36, 45, new Piece(TypePieces::PAWN, Color::BLACK), false, 37);
    EXPECT_EQ(board.getPieceAt("e5")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("f5")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("f6"), nullptr);
}


// ----------------------------------- OPTI -----------------------------------

// Test mouvement dame invalide
TEST_F(BoardTest, QueenMove) {
    board.clearBoard();
    board.placePiece("c5", new Piece(TypePieces::QUEEN, Color::WHITE));
    bool result = board.movePiece("c5g4"); // Dame blanche en g4
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt("c5")->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt("g4"), nullptr);
}

// Test mouvement dame invalide (bug que l'on a eu)
TEST_F(BoardTest, QueenMove2) {
    board.clearBoard();

    board.placePiece("a8", new Piece(TypePieces::ROOK, Color::BLACK));
    board.placePiece("b8", new Piece(TypePieces::KNIGHT, Color::BLACK));
    board.placePiece("e8", new Piece(TypePieces::KING, Color::BLACK));
    board.placePiece("f8", new Piece(TypePieces::BISHOP, Color::BLACK));
    board.placePiece("g8", new Piece(TypePieces::KNIGHT, Color::BLACK));
    board.placePiece("h8", new Piece(TypePieces::ROOK, Color::BLACK));

    board.placePiece("e7", new Piece(TypePieces::PAWN, Color::BLACK));
    board.placePiece("f7", new Piece(TypePieces::PAWN, Color::BLACK));
    board.placePiece("g7", new Piece(TypePieces::PAWN, Color::BLACK));

    board.placePiece("b6", new Piece(TypePieces::PAWN, Color::BLACK));
    board.placePiece("h6", new Piece(TypePieces::PAWN, Color::BLACK));

    board.placePiece("c5", new Piece(TypePieces::QUEEN, Color::BLACK));
    board.placePiece("d5", new Piece(TypePieces::PAWN, Color::BLACK));
    board.placePiece("e5", new Piece(TypePieces::PAWN, Color::WHITE));

    board.placePiece("g4", new Piece(TypePieces::QUEEN, Color::WHITE));

    board.placePiece("a3", new Piece(TypePieces::PAWN, Color::BLACK));
    board.placePiece("h3", new Piece(TypePieces::KNIGHT, Color::WHITE));

    board.placePiece("a2", new Piece(TypePieces::PAWN, Color::WHITE));
    board.placePiece("c2", new Piece(TypePieces::PAWN, Color::WHITE));
    board.placePiece("d2", new Piece(TypePieces::PAWN, Color::WHITE));
    board.placePiece("f2", new Piece(TypePieces::PAWN, Color::WHITE));
    board.placePiece("g2", new Piece(TypePieces::PAWN, Color::WHITE));
    board.placePiece("h2", new Piece(TypePieces::PAWN, Color::WHITE));

    board.placePiece("a1", new Piece(TypePieces::ROOK, Color::WHITE));
    board.placePiece("c1", new Piece(TypePieces::BISHOP, Color::WHITE));
    board.placePiece("e1", new Piece(TypePieces::KING, Color::WHITE));
    board.placePiece("f1", new Piece(TypePieces::BISHOP, Color::WHITE));
    board.placePiece("h1", new Piece(TypePieces::ROOK, Color::WHITE));

    bool result = board.movePiece("c5g4", Color::BLACK); // Dame noire en g4
    EXPECT_FALSE(result);
    EXPECT_EQ(board.getPieceAt("c5")->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt("c5")->getColor(), Color::BLACK);
    EXPECT_EQ(board.getPieceAt("g4")->getTypePiece(), TypePieces::QUEEN);
    EXPECT_EQ(board.getPieceAt("g4")->getColor(), Color::WHITE);
}




// Test CheckMate

TEST_F(BoardTest, BaiserDeLaMort)
{
    board.clearBoard();
    board.placePiece("h8", new Piece(TypePieces::KING, Color::WHITE)); // It has to get a king (even if useless)

    board.placePiece("b1", new Piece(TypePieces::KING, Color::BLACK));
    board.placePiece("c3", new Piece(TypePieces::QUEEN, Color::WHITE));
    board.placePiece("d3", new Piece(TypePieces::BISHOP, Color::WHITE));

    Color colWinner = Color::NONE;

    bool bResult = board.movePiece("c3b2", Color::WHITE);
    bool bIsKingCheck = board.isBlackKingCheck();
    bool bIsGameOver = board.isGameOver(Color::BLACK, colWinner);

    EXPECT_TRUE(bResult);
    EXPECT_TRUE(bIsKingCheck);
    EXPECT_TRUE(bIsGameOver);
    EXPECT_EQ(colWinner, Color::WHITE);
}

TEST_F(BoardTest, BaiserDeLaMortFailPatte)
{
    board.clearBoard();
    board.placePiece("h8", new Piece(TypePieces::KING, Color::WHITE)); // It has to get a king (even if useless)

    board.placePiece("b1", new Piece(TypePieces::KING, Color::BLACK));
    board.placePiece("a3", new Piece(TypePieces::QUEEN, Color::WHITE));
    board.placePiece("c4", new Piece(TypePieces::QUEEN, Color::WHITE));

    Color colWinner = Color::WHITE;

    bool bResult = board.movePiece("c4c3", Color::WHITE);
    bool bIsKingCheck = board.isBlackKingCheck();
    bool bIsGameOver = board.isGameOver(Color::BLACK, colWinner);

    EXPECT_TRUE(bResult);
    EXPECT_FALSE(bIsKingCheck);
    EXPECT_TRUE(bIsGameOver);
    EXPECT_EQ(colWinner, Color::NONE);
}

//Test exemple prof avec la tour qui protège et mets en échec
TEST_F(BoardTest, RookProtectKingAndAttack)
{
    board.clearBoard();
    board.placePiece("c2", new Piece(TypePieces::KING, Color::WHITE));
    board.placePiece("h7", new Piece(TypePieces::ROOK, Color::WHITE));

    board.placePiece("d7", new Piece(TypePieces::ROOK, Color::BLACK));
    board.placePiece("b7", new Piece(TypePieces::KING, Color::BLACK));

    bool bResult = board.movePiece("d7c7", Color::BLACK);
    bool bIsBlackKingCheck = board.isBlackKingCheck();
    bool bIsWhiteKingCheck = board.isWhiteKingCheck();

    EXPECT_TRUE(bResult);
    EXPECT_TRUE(bIsWhiteKingCheck);
    EXPECT_FALSE(bIsBlackKingCheck);
}

//Test en passant mets en échec le roi adverse
TEST_F(BoardTest, EnPassantMetEnEchecRoiAdverse)
{
    board.clearBoard();
    board.placePiece("h8", new Piece(TypePieces::KING, Color::WHITE)); // It has to get a king (even if useless)

    board.placePiece("e7", new Piece(TypePieces::KING, Color::BLACK));
    board.placePiece("d7", new Piece(TypePieces::PAWN, Color::BLACK));
    board.placePiece("e5", new Piece(TypePieces::PAWN, Color::WHITE));

    bool bResultBlackMove = board.movePiece("d7d5", Color::BLACK);
    bool bResultWhiteMove = board.movePiece("e5d6", Color::WHITE);
    bool bIsKingCheck = board.isBlackKingCheck();

    EXPECT_TRUE(bResultBlackMove);
    EXPECT_TRUE(bResultWhiteMove);
    EXPECT_TRUE(bIsKingCheck);
}

//Test en passant interdit car mets en échec son roi
TEST_F(BoardTest, EnPassantMetEnEchecSonRoiInterdit)
{
    board.clearBoard();
    board.placePiece("e1", new Piece(TypePieces::KING, Color::WHITE)); // It has to get a king (even if useless)
    board.placePiece("e5", new Piece(TypePieces::PAWN, Color::WHITE));

    board.placePiece("e7", new Piece(TypePieces::KING, Color::BLACK));
    board.placePiece("d7", new Piece(TypePieces::PAWN, Color::BLACK));
    board.placePiece("e6", new Piece(TypePieces::ROOK, Color::BLACK));

    bool bResultBlackMove = board.movePiece("d7d5", Color::BLACK);
    bool bResultWhiteMove = board.movePiece("e5d6", Color::WHITE);
    bool bIsBlackKingCheck = board.isBlackKingCheck();
    bool bIsWhiteKingCheck = board.isWhiteKingCheck();

    EXPECT_TRUE(bResultBlackMove);
    EXPECT_FALSE(bResultWhiteMove);
    EXPECT_FALSE(bIsBlackKingCheck);
    EXPECT_FALSE(bIsWhiteKingCheck);
}

//Test promotion met en échec roi adverse
TEST_F(BoardTest, PromotionDameMetRoiAdverseEchec)
{
    board.clearBoard();
    board.placePiece("e1", new Piece(TypePieces::KING, Color::WHITE)); // It has to get a king (even if useless)
    board.placePiece("e7", new Piece(TypePieces::PAWN, Color::WHITE));

    board.placePiece("g8", new Piece(TypePieces::KING, Color::BLACK));
    bool bResultWhiteMove = board.movePiece(Color::WHITE, "e7e8", nullptr, TypePieces::QUEEN);
    bool bIsBlackKingCheck = board.isBlackKingCheck();
    bool bIsWhiteKingCheck = board.isWhiteKingCheck();

    EXPECT_TRUE(bResultWhiteMove);
    EXPECT_TRUE(bIsBlackKingCheck);
    EXPECT_FALSE(bIsWhiteKingCheck);
}

//Test roi qui essaie de manger un pion derrière un autre roi (invalid move)
TEST_F(BoardTest, KingCantEatPawnDueToOtherKing)
{
    board.clearBoard();
    board.placePiece("e3", new Piece(TypePieces::KING, Color::WHITE));
    board.placePiece("e4", new Piece(TypePieces::PAWN, Color::WHITE));

    board.placePiece("e5", new Piece(TypePieces::KING, Color::BLACK));

    bool bResultBlackMove = board.movePiece("e5e4",Color::BLACK);
    bool bIsBlackKingCheck = board.isBlackKingCheck();
    bool bIsWhiteKingCheck = board.isWhiteKingCheck();

    EXPECT_FALSE(bResultBlackMove);
    EXPECT_FALSE(bIsBlackKingCheck);
    EXPECT_FALSE(bIsWhiteKingCheck);
}

//Test NumberOfMove at the begining of the game
TEST_F(BoardTest, GoodNumberOfMoveAtTheBeginingOfTheGame)
{
    std::vector<std::pair<int, int>> listOfPossibleWhiteMoves = board.listOfPossibleMoves(Color::WHITE);
    std::vector<std::pair<int, int>> listOfPossibleBlackMoves = board.listOfPossibleMoves(Color::BLACK);

    EXPECT_EQ(listOfPossibleWhiteMoves.size(), 20);
    EXPECT_EQ(listOfPossibleBlackMoves.size(), 20);
}

//Test Bug we had when the black moves, it promotes a white pawn to a queen
TEST_F(BoardTest, BugPawnPromoteToQueenWhenEnemyMoves)
{
    bool bMove1 = board.movePiece("e2e4");
    bool bMove2 = board.movePiece("f7f5", Color::BLACK);
    bool bMove3 = board.movePiece("e4f5");
    bool bMove4 = board.movePiece("g7g6", Color::BLACK);
    bool bIsBlackKingCheck = board.isBlackKingCheck();
    bool bIsWhiteKingCheck = board.isWhiteKingCheck();

    EXPECT_TRUE(bMove1);
    EXPECT_TRUE(bMove2);
    EXPECT_TRUE(bMove3);
    EXPECT_TRUE(bMove4);

    EXPECT_EQ(board.getPieceAt("f5")->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(board.getPieceAt("f5")->getColor(), Color::WHITE);
    EXPECT_FALSE(bIsBlackKingCheck);
    EXPECT_FALSE(bIsWhiteKingCheck);
}

//Test Bug we had when the white pawn can move in diagonal after e2e4
TEST_F(BoardTest, BugPawnDiagonalMove)
{
    bool bMove1 = board.movePiece("e2e4");

    std::vector<std::pair<int, int>> listOfPossibleWhiteMoves = board.listOfPossibleMoves(Color::WHITE);

    EXPECT_EQ(listOfPossibleWhiteMoves.size(), 30);
    EXPECT_TRUE(bMove1);
}

// Bug que l'on a eu où le roi ne pouvait pas manger
TEST_F(BoardTest, BugKingCantEat) {
    // Libère le chemin pour le grand roque noir
    board.setupFromFEN("rnbqkbn1/3ppp2/p1p3p1/1p6/3N2P1/4P1P1/PPPPPP2/RNB1Kr2 w kq - 0 1");

    bool result2 = board.movePiece("e1f1");
    EXPECT_TRUE(result2);
    EXPECT_EQ(board.getPieceAt("e1"), nullptr);
    EXPECT_EQ(board.getPieceAt("f1")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("f1")->getColor(), Color::WHITE);
}

// Test pour voir si le pion attaque de loin en diagonale (plus d'une ligne)
TEST_F(BoardTest, PawnAttackFromFarDistance)
{
    board.clearBoard();

    board.placePiece("c2", new Piece(TypePieces::KING, Color::WHITE));
    board.placePiece("e8", new Piece(TypePieces::KING, Color::BLACK));
    board.placePiece("e5", new Piece(TypePieces::PAWN, Color::BLACK));

    bool result2 = board.movePiece("c2b2");
    int iPositionPieceFound = -1;
    Piece* pPieceThatAttackKing = board.findFirstPieceOnDirectionThatAttacksInitialPosition(9, 9, 7, iPositionPieceFound); // Verify who attacks the king on the right up diagonal
    EXPECT_TRUE(result2);
    EXPECT_EQ(pPieceThatAttackKing, nullptr);
    EXPECT_EQ(iPositionPieceFound, -1);
    EXPECT_EQ(board.getPieceAt("c2"), nullptr);
    EXPECT_EQ(board.getPieceAt("b2")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("b2")->getColor(), Color::WHITE);
}

// Test pour voir si le pion attaque de loin en diagonale (plus d'une ligne)
TEST_F(BoardTest, KingMoveBehindEnemyPAwn)
{
    board.clearBoard();

    board.placePiece("e6", new Piece(TypePieces::KING, Color::WHITE));
    board.placePiece("e8", new Piece(TypePieces::KING, Color::BLACK));
    board.placePiece("e5", new Piece(TypePieces::PAWN, Color::BLACK));

    bool result2 = board.movePiece("e6f6");
    EXPECT_TRUE(result2);
    EXPECT_EQ(board.getPieceAt("e6"), nullptr);
    EXPECT_EQ(board.getPieceAt("f6")->getTypePiece(), TypePieces::KING);
    EXPECT_EQ(board.getPieceAt("f6")->getColor(), Color::WHITE);
}