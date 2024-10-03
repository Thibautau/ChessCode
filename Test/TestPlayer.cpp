#include <gtest/gtest.h>
#include "Player.cpp"

TEST(PlayerTest, PlayerInitialisation) {
    Player* player = new Player(Color::WHITE);
    player->setScore(3);
    EXPECT_EQ(player->getPlayerColor(), Color::WHITE);
    EXPECT_EQ(player->getScore(), 3);
}

TEST(PlayerTest, ValideCoordinate) {
    Player* player = new Player(Color::WHITE);
    EXPECT_EQ(player->isValidCoordinate('e', '4'), true);
    EXPECT_EQ(player->isValidCoordinate('i', '4'), false);
    EXPECT_EQ(player->isValidCoordinate('e', '9'), false);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "Démarrage des tests..." << std::endl;
    return RUN_ALL_TESTS();
}
