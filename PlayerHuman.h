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
    Color getPlayerColor() const;
    void setPlayerColor(Color color);
    int getScore() const;
    void setScore(int in_iScore);
    void play(Board& board, Coordinate& start, Coordinate& end) override;
    static void getInput(std::string& input);
    bool isInputValid(const std::string& in_sInput, Coordinate& out_start, Coordinate& out_end);
    static bool isValidCoordinate(char col, char row);
};

#endif // PLAYERHUMAN_H
