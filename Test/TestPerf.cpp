//
// Created by Peter on 04/11/2024.
//

#include <iostream>
#include <chrono>
#include "TestPerf.h"
#include "Bot.cpp"
#include "Board.cpp"
#include "Zobrist.cpp"
#include "Piece.cpp"

#include <iostream>
#include <chrono>

void testPerformance(Board& board, const std::string& fen, int profondeur_max) {
    board.setupFromFEN(fen);

    Bot bot(Color::WHITE);
    std::pair<int, int> meilleurCoup;

    auto start = std::chrono::high_resolution_clock::now();
    bot.choisir_meilleur_coup(board, profondeur_max, meilleurCoup);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "FEN: " << fen << "\n";
    std::cout << "Meilleur coup: " << meilleurCoup.first << ", " << meilleurCoup.second << "\n";
    std::cout << "Node: " << Bot::nodeCount << " \n";
    std::cout << "Temps pris: " << duration.count() << " ms\n";
    std::cout << "Noeuds evalues par seconde: " << (Bot::nodeCount / (duration.count() / 1000)) << "\n \n";
}

int main(int argc, char **argv) {
    Board board;

    // Test Early Game
    testPerformance(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);

    // Test Mid Game
    testPerformance(board, "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/8/PPPP1PPP/RNBQK1NR w KQkq - 0 4", 6);

    // Test Late Game
    testPerformance(board, "8/8/4k3/2Q2P2/4K3/4p3/8/8 w - - 0 1", 6);

    return 0;
}

