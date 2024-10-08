//
// Created by Peter on 02/10/2024.
//

#include "Bot.h"
#include <cstdlib>

Bot::Bot(Color color) : m_color(color) {}

Color Bot::getPlayerColor() const {
    return m_color;
}

void Bot::play(Board& board, int& start, int& end) {
    /*std::vector<int> possibleMoves = board.listOfPossibleMoves(m_color);

    if (possibleMoves.empty()) {
        return;
    }

    int randomIndex = rand() % possibleMoves.size();

    start = possibleMoves[randomIndex];
    end = possibleMoves[randomIndex];*/
}


