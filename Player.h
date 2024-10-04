//
// Created by Peter on 03/10/2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "Type.h"

class Player {
public:
    virtual void play(Board& board, Coordinate& start, Coordinate& end) = 0;
    virtual Color getPlayerColor() const = 0;
    virtual ~Player();
};

#endif //PLAYER_H
