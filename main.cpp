
#include "MainChessGame.h"
#include <iostream>

#include "UCI/UCI.h"

int main() {
    const GameMode mode = GameMode::JVB;
    MainChessGame* game = new MainChessGame(mode);
    game->initChessGame();

    /*bool bUseUCI = true;
    if(bUseUCI)
    {*/
        UCI::uciCommunication();
    //}

    while (!game->isGameOver()) {
        game->playTurn();
    }

    const Color colWinner = game->getColorWinner();
    if (colWinner == Color::WHITE) {
        std::cout << "Les Blancs ont gagné !" << std::endl;
    }
    else if (colWinner == Color::BLACK) {
        std::cout << "Les Noirs ont gagné !" << std::endl;
    }
    else {
        std::cout << "Match nul !" << std::endl;
    }

    delete game;

    return 0;
}
