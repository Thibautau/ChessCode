//
// Created by Peter on 02/10/2024.
//

#include "Bot.h"
#include <cstdlib>

Bot::Bot(Color color) {
    m_color = color;
}

Coordinate Bot::playRandomMove(Board &board) {
    std::vector<Move> possibleMoves = board.listOfPossibleMoves(m_color);
    if (possibleMoves.empty()) {
        // Pas de mouvements possibles
        return {};
    }
    int randomIndex = rand() % possibleMoves.size();
    return possibleMoves[randomIndex].coordStart;
}

