#include <gtest/gtest.h>
#include "PlayerHuman.cpp"
#include "Type.h"

TEST(PlayerTest, PlayerInitialisation) {
    PlayerHuman* player = new PlayerHuman(Color::WHITE);
    player->setScore(3);
    EXPECT_EQ(player->getPlayerColor(), Color::WHITE);
    EXPECT_EQ(player->getScore(), 3);
}

TEST(PlayerTest, ValideInput) {
    PlayerHuman* player = new PlayerHuman(Color::WHITE);
    std::string input = "e2 e4";
    Coordinate start = Coordinate(1, 4);
    Coordinate end = Coordinate(3, 4);
    EXPECT_EQ(player->isInputValid(input, start, end), true);
    input = "e2  e4";
    EXPECT_EQ(player->isInputValid(input, start, end), false);
}

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
