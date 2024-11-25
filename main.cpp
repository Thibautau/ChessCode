
#include "MainChessGame.h"
#include <iostream>

#include "UCI/UCI.h"

int main() {
    const GameMode mode = GameMode::JVB;
    MainChessGame* game = new MainChessGame(mode);
    game->initChessGame();

    game->getBoard()->clearBoard();
    game->getBoard()->setupFromFEN("8/8/k7/2Q5/2K5/8/8/8 w - - 21 13");

    /*bool bUseUCI = true;
    if(bUseUCI)
    {*/
        UCI* uci = new UCI();
        uci->uciCommunication();
    //}

    while (!game->isGameOver()) {
        //game->playTurn();
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
