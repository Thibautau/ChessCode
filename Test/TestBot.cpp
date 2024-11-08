//
// Created by thiba on 07/11/2024.
//

#include <iostream>
#include <chrono>
#include "Bot.h"
#include "Board.h"
#include "Zobrist.h"
#include <gtest/gtest.h>
#include "MainChessGame.h"


class TestBot : public ::testing::Test {
protected:
    MainChessGame* game = nullptr;
    Board* board = nullptr;
    Bot* whiteBot = nullptr;
    Bot* blackBot = nullptr;

    void SetUp() override {
        game = new MainChessGame(GameMode::BVJ);
        game->initChessGame();
        board = game->getBoard();
        whiteBot = new Bot(Color::WHITE);
        blackBot = new Bot(Color::BLACK);
    }
};
/**
 * Exécute un test de performance pour le bot d'échecs en calculant le meilleur coup pour une position donnée.
 * @param game - Référence vers la partie de jeu d'échecs (MainChessGame)
 * @param fen - Chaîne FEN décrivant la position à analyser
 * @param profondeur_max - Profondeur maximale de recherche pour le bot
 */
void testPerformance(MainChessGame& game, const std::string fen, int profondeur_max) {
    game.setBoardFromFEN(fen);
    Board *board = game.getBoard();

    Bot bot(Color::WHITE);
    std::pair<int, int> meilleurCoup;

    auto start = std::chrono::high_resolution_clock::now();
    bot.choisir_meilleur_coup(*board, profondeur_max, meilleurCoup);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "FEN: " << fen << "\n";
    std::cout << "Meilleur coup: " << meilleurCoup.first << ", " << meilleurCoup.second << "\n";
    std::cout << "Node: " << Bot::nodeCount << " \n";
    std::cout << "Temps pris: " << duration.count() << " ms\n";
    std::cout << "Noeuds evalues par seconde: " << (Bot::nodeCount / (duration.count() / 1000)) << "\n \n";
}

// Voir si on a parcouru le bon nombre de noeud (pour aussi voir si la table de transposition fonctionne)
TEST_F(TestBot, GoodNumberOfNodeIterated) {
    game->setBoardFromFEN("k7/8/8/8/8/8/8/K7");

    std::pair<int, int> meilleurCoup;
    whiteBot->choisir_meilleur_coup(*board, 6,meilleurCoup);

    std::cout << "UniqueNode: " << Bot::uniqueNodeIterated << " \n";
    std::cout << "Node: " << Bot::nodeCount << " \n";
    EXPECT_EQ(Bot::uniqueNodeIterated, 8);
}
