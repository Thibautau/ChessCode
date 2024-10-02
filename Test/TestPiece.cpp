#include <gtest/gtest.h>
#include "Piece.cpp"

// Définir un test Google Test
TEST(PieceTest, HandlesPositiveNumbers) {
    Piece *piece = new Piece(TypePieces::PAWN, Color::WHITE);
    EXPECT_EQ(piece->getTypePiece(), TypePieces::PAWN);
    EXPECT_EQ(piece->getColor(), Color::WHITE);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "Démarrage des tests..." << std::endl;
    return RUN_ALL_TESTS();
}
