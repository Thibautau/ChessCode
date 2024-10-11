//
// Created by Peter on 02/10/2024.
//

#include "Bot.h"
#include <cstdlib>
#include <math.h>
#include <limits>

Bot::Bot(Color color) : m_color(color) {}

Color Bot::getPlayerColor() const {
    return m_color;
}

void Bot::play(Board& board, int& start, int& end) {
    std::pair<int, int> meilleurCoup;
    int profondeur_max = 4;

    choisir_meilleur_coup(board, profondeur_max, meilleurCoup);

    start = meilleurCoup.first;
    end = meilleurCoup.second;
}

void Bot::choisir_meilleur_coup(Board& board, int profondeur_max,std::pair<int, int>& meilleurCoup) {
    int meilleurScore = std::numeric_limits<int>::min();
    meilleurCoup = { -1, -1 };

    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(m_color);

    for(const std::pair<int, int>& coup : possibleMoves) {
        Piece* capturedPiece = nullptr;
        //@TODO Pas très opti je pense, faut trouver une autre solution pour gérer la promotion
        Piece* movingPiece = board.getPieceAt(coup.first);
        bool isPromotion = (movingPiece->getTypePiece() == TypePieces::PAWN) &&((m_color == Color::WHITE && coup.second / 8 == 7) ||(m_color == Color::BLACK && coup.second / 8 == 0));
        board.movePiece(coup.first, coup.second, m_color, &capturedPiece,TypePieces::QUEEN);
        int score = minimax(board, profondeur_max - 1, false);
        board.undoMove(coup.first, coup.second, capturedPiece, isPromotion);
        if(score > meilleurScore) {
            meilleurScore = score;
            meilleurCoup = coup;
        }
    }
}

int Bot::minimax(Board& board, int profondeur, bool estMaximisant) {
    if(profondeur == 0 || board.isGameOver(m_color)) {
        return board.evaluate(m_color);
    }

    if(estMaximisant) {
        int meilleurScore = std::numeric_limits<int>::min();
        std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(m_color);

        for(const std::pair<int, int>& coup : possibleMoves) {
            Piece* capturedPiece = nullptr;
            //@TODO Pas très opti je pense, faut trouver une autre solution pour gérer la promotion
            Piece* movingPiece = board.getPieceAt(coup.first);
            bool isPromotion = (movingPiece->getTypePiece() == TypePieces::PAWN) &&((m_color == Color::WHITE && coup.second / 8 == 7) ||(m_color == Color::BLACK && coup.second / 8 == 0));

            board.movePiece(coup.first, coup.second, m_color, &capturedPiece,TypePieces::QUEEN);
            int score = minimax(board, profondeur - 1, false);
            board.undoMove(coup.first, coup.second, capturedPiece,isPromotion);
            meilleurScore = std::max(meilleurScore, score);
        }

        return meilleurScore;
    }
    else {
        int meilleurScore = std::numeric_limits<int>::max();
        Color adversaire = (m_color == Color::WHITE) ? Color::BLACK : Color::WHITE;
        std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(adversaire);

        for(const std::pair<int, int>& coup : possibleMoves) {
            Piece* capturedPiece = nullptr;
            //@TODO Pas très opti je pense, faut trouver une autre solution pour gérer la promotion
            Piece* movingPiece = board.getPieceAt(coup.first);
            bool isPromotion = (movingPiece->getTypePiece() == TypePieces::PAWN) &&((m_color == Color::WHITE && coup.second / 8 == 7) ||(m_color == Color::BLACK && coup.second / 8 == 0));
            board.movePiece(coup.first, coup.second, adversaire,&capturedPiece,TypePieces::QUEEN);
            int score = minimax(board, profondeur - 1, true);
            board.undoMove(coup.first, coup.second, capturedPiece, isPromotion);
            meilleurScore = std::min(meilleurScore, score);
        }

        return meilleurScore;
    }
}


