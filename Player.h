//
// Created by Peter on 02/10/2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "Type.h"
#include "Board.h"
#include <string>


class Player {
protected:
    Color m_colPlayerColor;
    int m_iScore;

public:
    Player(Color color);
    Color getPlayerColor() const;
    void setPlayerColor(Color Color);

    int getScore() const;
    void setScore(int in_iScore);
    Coordinate play();
    std::string getInput();
    bool isInputValid(const std::string& in_sInput,int& out_iColumn, int& out_iRow);
};

#endif //PLAYER_H
