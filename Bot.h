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
    void choisir_meilleur_coup(Board& board, int profondeur_max,std::pair<int, int>& meilleurCoup);
    int minimax(Board& board, int profondeur, bool estMaximisant, int alpha, int beta);
};

#endif //BOT_H
