//
// Created by Peter on 03/10/2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "Type.h"

class Player {
public:
    virtual void play(Board& board, int& start, int& end) = 0;
    virtual void playWithDepth(Board& board, int& start, int& end, int depth);
    virtual Color getPlayerColor() const;
    //virtual ~Player();
};

#endif //PLAYER_H
