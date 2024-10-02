
#include "MainChessGame.h"
#include <iostream>

int main() {
    MainChessGame game;
    game.initChessGame();

    while (!game.isGameOver()) {
        game.playTurn();
    }

    Color winner = game.getWinner();
    if (winner == Color::WHITE) {
        std::cout << "Les Blancs ont gagné !" << std::endl;
    } else if (winner == Color::BLACK) {
        std::cout << "Les Noirs ont gagné !" << std::endl;
    } else {
        std::cout << "Match nul !" << std::endl;
    }

    return 0;
}
