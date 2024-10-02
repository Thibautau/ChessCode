#include <gtest/gtest.h>
#include "Player.cpp"

// Définir un test Google Test
TEST(PlayerTest, HandlesPositiveNumbers) {
    Player* player = new Player(Color::WHITE);
    player->setScore(3);
    EXPECT_EQ(player->getPlayerColor(), Color::WHITE);
    EXPECT_EQ(player->getScore(), 3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "Démarrage des tests..." << std::endl;
    return RUN_ALL_TESTS();
}
