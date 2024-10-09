//
// Created by Peter on 02/10/2024.
//

#ifndef BOT_H
#define BOT_H

#include "Type.h"
#include "Board.h"
#include "Player.h"

class Bot : public Player {
private:
    Color m_color;

public:
    Bot(Color color);
    void play(Board& board, Coordinate& start, Coordinate& end) override;
    Move chooseBestMove(Board& board, int max_depth);
    int minimax(Board& board, int depth, bool isMax);
    Color getPlayerColor() const override;
};

#endif //BOT_H
