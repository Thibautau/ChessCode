
#include "MainChessGame.h"
#include <iostream>

int main() {
    MainChessGame game;
    game.initChessGame();

    while (!game.isGameOver())
    {
        game.playTurn();
    }

    Color colWinner = game.getColorWinner();
    if (colWinner == Color::WHITE) {
        std::cout << "Les Blancs ont gagné !" << std::endl;
    } else if (colWinner == Color::BLACK) {
        std::cout << "Les Noirs ont gagné !" << std::endl;
    } else {
        std::cout << "Match nul !" << std::endl;
    }

    return 0;
}
