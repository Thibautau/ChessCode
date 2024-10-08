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
    void play(Board& board, int& start, int& end) override;
    Color getPlayerColor() const override;
};

#endif //BOT_H
