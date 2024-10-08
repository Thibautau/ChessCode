#include <gtest/gtest.h>
#include "PlayerHuman.cpp"
#include "Piece.cpp"

//Initialisation du joueur
TEST(PlayerTest, PlayerInitialisation) {
    PlayerHuman* player = new PlayerHuman(Color::WHITE);
    player->setScore(3);
    EXPECT_EQ(player->getPlayerColor(), Color::WHITE);
    EXPECT_EQ(player->getScore(), 3);
}

//Verification de l'input
TEST(PlayerTest, ValideInput) {
    PlayerHuman* player = new PlayerHuman(Color::WHITE);
    std::string input = "e2e4";
    Coordinate start = Coordinate(1, 4);
    Coordinate end = Coordinate(3, 4);
    int iStart = 12, iEnd = 28;
    EXPECT_EQ(player->isInputValid(input, iStart, iEnd), true);
    input = "e2 e4";
    EXPECT_EQ(player->isInputValid(input, iStart, iEnd), false);
    input = "e2e10";
    EXPECT_EQ(player->isInputValid(input, iStart, iEnd), false);
}

//Verification des coordonnées
TEST(PlayerTest, ValideCoordinate) {
    PlayerHuman* player = new PlayerHuman(Color::WHITE);
    EXPECT_EQ(player->isValidCoordinate('e', '4'), true);
    EXPECT_EQ(player->isValidCoordinate('i', '4'), false);
    EXPECT_EQ(player->isValidCoordinate('e', '9'), false);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "Démarrage des tests..." << std::endl;
    return RUN_ALL_TESTS();
}
