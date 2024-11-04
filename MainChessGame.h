//
// Created by Peter on 02/10/2024.
//

#ifndef MAINCHESSGAME_H
#define MAINCHESSGAME_H

#include "PlayerHuman.h"
#include "Board.h"
#include "Type.h"

class MainChessGame {
private:
    Player* m_currentPlayer;
    Player* m_waitingPlayer;
    Board* m_board;
    bool m_isGameOver;
    Color m_colorWinner;

public:
    explicit MainChessGame(GameMode mode);
    ~MainChessGame();

    void initChessGame() const;

    std::string indexToPosition(int pos);
    void debugPrintMove(int start, int end);

    void playTurn();
    void changeCurrentPlayer();

    bool isGameOver();
    Color getColorWinner() const;

    Player* getCurrentPlayer() const;
    Player* getWaitingPlayer() const;

    void initChessGameFromFEN(const std::string& fen);

    Board* getBoard() const;
};

#endif //MAINCHESSGAME_H
