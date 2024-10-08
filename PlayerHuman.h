//
// Created by Peter on 02/10/2024.
//

#ifndef PLAYERHUMAN_H
#define PLAYERHUMAN_H

#include "Type.h"
#include "Board.h"
#include "Player.h"
#include <string>


class PlayerHuman : public Player {
protected:
    Color m_colPlayerColor;
    int m_iScore;

public:
    PlayerHuman(Color color);
    Color getPlayerColor() const override;
    void setPlayerColor(Color color);
    int getScore() const;
    void setScore(int in_iScore);
    void play(Board& board, int& start, int& end) override;
    static void getInput(std::string& input);
    bool isInputValid(const std::string& in_sInput, int& out_start, int& out_end);
    static bool isValidCoordinate(char col, char row);
};

#endif // PLAYERHUMAN_H
