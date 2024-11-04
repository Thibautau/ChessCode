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
    std::cout << "Temps pris: " << duration.count() << " ms\n";
    std::cout << "Noeuds evalues par seconde: " << (Bot::nodeCount / (duration.count() / 1000)) << "\n \n";
}

int main(int argc, char **argv) {
    Board board;

    // Test Early Game
    testPerformance(board, "rnbqkb1r/pppppppp/8/8/4P3/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 3);

    // Test Mid Game
    testPerformance(board, "r1bqkbnr/ppp1pppp/2n5/3p4/4P3/2N5/PPP1PPPP/R1BQKBNR w KQkq - 0 1", 3);

    // Test Late Game
    testPerformance(board, "8/8/8/8/8/8/5k2/6K1 w - - 0 1", 3);

    return 0;
}

