//
// Created by Peter on 02/10/2024.
//

#ifndef MAINCHESSGAME_H
#define MAINCHESSGAME_H

#include <memory>
#include "Player.h"
#include "Board.h"
#include "Type.h"

class ChessGame {
private:
    std::unique_ptr<Player> m_currentPlayer;
    std::unique_ptr<Player> m_waitingPlayer;
    Board m_board;
    bool m_isGameOver;
    Color m_winner;

public:
    ChessGame(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2);

    void initChessGame();
    void playTurn();
    void changeCurrentPlayer();

    bool isGameOver() const;
    Color getWinner() const;

    Player& getCurrentPlayer();
    Player& getWaitingPlayer();

    Board& getBoard();
};

#endif //MAINCHESSGAME_H
