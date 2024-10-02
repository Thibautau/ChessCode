#include <gtest/gtest.h>

// Test d'initialisation du jeu
TEST(ChessGameTest, InitializationTest) {
    MainChessGame game;
    game.startGame();
    EXPECT_EQ(game.initChessGame(), true);
}

// Test de mouvements de base
TEST(ChessGameTest, ValidMoveTest) {
    MainChessGame game;
    game.startGame();
    EXPECT_TRUE(game.board.playMove("e2 e4"));
    EXPECT_EQ(game.getPieceAt("e4").getType(), PieceType::PAWN);
}

// Test de mouvements de base invalide
TEST(ChessGameTest, InvalidMoveTest) {
    MainChessGame game;
    game.startGame();
    EXPECT_FALSE(game.makeMove("e2 e5"));
}

//Tester mouvement valide et invalide de toutes les pièces

//Mouvement d'un pion après l'avoir déjà jouer

//Manger un pion enemy si avance de deux (cas spécial)

//Manger un enemy

//Ne pas passer à travers les pièces

//Tester le rock avec possible et impossible et tester les positions

//Tester les mouvement quand checkmate


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "Démarrage des tests..." << std::endl;
    return RUN_ALL_TESTS();
}


