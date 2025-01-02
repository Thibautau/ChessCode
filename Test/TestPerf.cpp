//
// Created by Peter on 04/11/2024.
//

#include <iostream>
#include <chrono>
#include "TestPerf.h"
#include "Bot.cpp"
#include "Board.cpp"
#include "MainChessGame.cpp"
#include "PlayerHuman.cpp"
#include "Zobrist.cpp"
#include "Piece.cpp"
#include "OpeningBook/OpeningBook.cpp"
#include "OpeningBook/ReadOpeningBook.cpp"

#include <iostream>
#include <chrono>

#include "MainChessGame.h"



/**
 * Exécute un test de performance pour le bot d'échecs en calculant le meilleur coup pour une position donnée.
 * @param game - Référence vers la partie de jeu d'échecs (MainChessGame)
 * @param fen - Chaîne FEN décrivant la position à analyser
 * @param profondeur_max - Profondeur maximale de recherche pour le bot
 */
void testPerformance(MainChessGame& game, const std::string fen, int profondeur_max) {
    game.setBoardFromFEN(fen);
    Board *board = game.getBoard();

    Bot* bot = new Bot(Color::WHITE);
    std::pair<int, int> meilleurCoup;

    auto start = std::chrono::high_resolution_clock::now();
    bot->choisir_meilleur_coupv2(*board, profondeur_max, meilleurCoup);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "FEN: " << fen << "\n";
    std::cout << "Meilleur coup: " << meilleurCoup.first << ", " << meilleurCoup.second << "\n";
    std::cout << "Node: " << Bot::nodeCount << " \n";
    std::cout << "Temps pris: " << duration.count() << " ms\n";
    std::cout << "Noeuds evalues par seconde: " << (Bot::nodeCount / (duration.count() / 1000)) << "\n \n";

    delete bot;
    bot = nullptr;
}

/**
 * Fonction main qui exécute plusieurs tests de performance sur des positions de début, milieu et fin de partie.
 */
int main() {
    MainChessGame *game = new MainChessGame(GameMode::JVB);

    // Test Early Game
    testPerformance(*game, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);
    //testPerformance(*game, "rnbqkbnr/pppp1ppp/8/4p3/4P3/5Q2/PPPP1PPP/RNB1KBNR w KQkq - 0 1", 1);

    // Test Mid Game
    //testPerformance(*game, "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/8/PPPP1PPP/RNBQK1NR w KQkq - 0 4", 4);

    // Test Late Game
    //testPerformance(*game, "8/8/4k3/2Q2P2/4K3/4p3/8/8 w - - 0 1", 4);
    ////testPerformance(*game, "6k1/5ppp/8/8/8/8/5PPP/6QK w - - 0 1", 6);
    //testPerformance(*game, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 6);

    delete game;
    game = nullptr;

    return 0;
}

