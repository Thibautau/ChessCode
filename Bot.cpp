//
// Created by Peter on 02/10/2024.
//

#include "Bot.h"
#include <cstdlib>

Bot::Bot(Color color) : m_color(color) {}

Color Bot::getPlayerColor() const {
    return m_color;
}

std::pair<Coordinate, Coordinate> Bot::play(Board &board) {
    std::vector<Move> possibleMoves = board.listOfPossibleMoves(m_color);
    if (possibleMoves.empty()) {
        return {}; // Pas de mouvement possible
    }
    int randomIndex = rand() % possibleMoves.size();
    return {possibleMoves[randomIndex].coordStart, possibleMoves[randomIndex].coordEnd};
}


