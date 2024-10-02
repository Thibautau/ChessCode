#include <gtest/gtest.h>
#include "Piece.cpp"

// Définir un test Google Test
TEST(PieceTest, HandlesPositiveNumbers) {
    Piece* piece = new Piece(TypePieces::PAWN, Color::WHITE);
    EXPECT_EQ(piece->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(piece->getColor(), Color::WHITE);
}
