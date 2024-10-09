//
// Created by Peter on 02/10/2024.
//

#include "Bot.h"
#include <cstdlib>
#include <math.h>

Bot::Bot(Color color) : m_color(color) {}

Color Bot::getPlayerColor() const {
    return m_color;
}

void Bot::play(Board& board, int& start, int& end) {
    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(m_color);

    if (possibleMoves.empty()) {
        return;
    }

    int randomIndex = rand() % possibleMoves.size();

    start = possibleMoves[randomIndex].first;
    end = possibleMoves[randomIndex].second;
}

std::pair<int, int> Bot::choisir_meilleur_coup(Board& board, int profondeur_max) {
    int meilleurScore = -1;
    std::pair<int, int> meilleurCoup = { -1, -1 };

    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(m_color);

    for(const std::pair<int, int>& coup : possibleMoves) {
        board.movePiece(coup.first, coup.second, m_color);
        int score = minimax(board, profondeur_max - 1, false);
        board.undoMove();
        if(score > meilleurScore) {
            meilleurScore = score;
            meilleurCoup = coup;
        }
    }

    return meilleurCoup;
}

int Bot::minimax(Board& board, int profondeur, bool estMaximisant) {
    if(profondeur == 0 || board.isGameOver()) {
        return board.evaluate(m_color);
    }

    if(estMaximisant) {
        int meilleurScore = -INFINITY;
        std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(m_color);

        for(const std::pair<int, int>& coup : possibleMoves) {
            board.movePiece(coup.first, coup.second, m_color);
            int score = minimax(board, profondeur - 1, false);
            board.undoMove();
            meilleurScore = std::max(meilleurScore, score);
        }

        return meilleurScore;
    }
    else {
        int meilleurScore = INFINITY;
        Color adversaire = (m_color == Color::WHITE) ? Color::BLACK : Color::WHITE;
        std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(adversaire);

        for(const std::pair<int, int>& coup : possibleMoves) {
            board.movePiece(coup.first, coup.second, adversaire);
            int score = minimax(board, profondeur - 1, true);
            board.undoMove();
            meilleurScore = std::min(meilleurScore, score);
        }

        return meilleurScore;
    }
}


