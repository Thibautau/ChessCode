//
// Created by Peter on 03/10/2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "Type.h"

class Player {
public:
    virtual std::pair<Coordinate, Coordinate> play(Board& board) = 0;
    virtual Color getPlayerColor() const = 0;
};

#endif //PLAYER_H
