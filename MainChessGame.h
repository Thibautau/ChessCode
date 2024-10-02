//
// Created by Peter on 02/10/2024.
//

#ifndef MAINCHESSGAME_H
#define MAINCHESSGAME_H

#include <memory>
#include "Player.h"
#include "Board.h"
#include "Type.h"

class MainChessGame {
private:
    Player* m_currentPlayer;
    Player* m_waitingPlayer;
    Board* m_board;
    bool m_isGameOver;
    Color m_winner;

public:
    MainChessGame();

    void initChessGame();
    void playTurn();
    void changeCurrentPlayer();

    bool isGameOver();
    Color getWinner();

    Player& getCurrentPlayer();
    Player& getWaitingPlayer();

    Board& getBoard();
};

#endif //MAINCHESSGAME_H
