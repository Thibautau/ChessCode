//
// Created by Peter on 02/10/2024.
//

#ifndef BOT_H
#define BOT_H

#include "Type.h"
#include "Board.h"

class Bot {
private:
    Color m_color;

public:
    Bot(Color color);
    Coordinate playRandomMove(Board& board);
};

#endif //BOT_H
