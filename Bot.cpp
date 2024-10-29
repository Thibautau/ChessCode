//
// Created by Peter on 02/10/2024.
//

#include "Bot.h"
#include <cstdlib>
#include <math.h>
#include <limits>
#include <algorithm>

Bot::Bot(Color color) : m_color(color) {}

Color Bot::getPlayerColor() const {
    return m_color;
}

void Bot::play(Board& board, int& start, int& end) {
    std::pair<int, int> meilleurCoup;
    int profondeur_max = 6;

    choisir_meilleur_coup(board, profondeur_max, meilleurCoup);

    start = meilleurCoup.first;
    end = meilleurCoup.second;
}

void Bot::choisir_meilleur_coup(Board& board, int profondeur_max, std::pair<int, int>& meilleurCoup) {
    int meilleurScore = std::numeric_limits<int>::min();
    meilleurCoup = { -1, -1 };

    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(m_color);
    std::sort(possibleMoves.begin(), possibleMoves.end(), [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return board.evaluateMove(a, m_color) > board.evaluateMove(b, m_color);
    });

    for (const std::pair<int, int>& coup : possibleMoves) {
        Piece* capturedPiece = nullptr;
        bool isPromotion = board.isPromotionMove(coup.first, coup.second, m_color);
        int enPassantPos = -1;

        board.movePiece(coup.first, coup.second, m_color, &capturedPiece, TypePieces::QUEEN,&enPassantPos);
        int score = minimax(board, profondeur_max - 1, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        board.undoMove(coup.first, coup.second, capturedPiece, isPromotion);
        if (score > meilleurScore) {
            meilleurScore = score;
            meilleurCoup = coup;
        }
    }
}

int Bot::minimax(Board& board, int profondeur, bool estMaximisant, int alpha, int beta) {
    if (profondeur == 0) {
        return board.evaluate(m_color);
    }

    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(estMaximisant ? m_color : (m_color == Color::WHITE ? Color::BLACK : Color::WHITE));
    std::sort(possibleMoves.begin(), possibleMoves.end(), [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return board.evaluateMove(a, m_color) > board.evaluateMove(b, m_color);
    });

    if (estMaximisant) {
        int meilleurScore = std::numeric_limits<int>::min();
        for (const std::pair<int, int>& coup : possibleMoves) {
            Piece* capturedPiece = nullptr;
            bool isPromotion = board.isPromotionMove(coup.first, coup.second, m_color);
            int enPassantPos = -1;

            board.movePiece(coup.first, coup.second, m_color, &capturedPiece, TypePieces::QUEEN,&enPassantPos);
            int score = minimax(board, profondeur - 1, false, alpha, beta);
            board.undoMove(coup.first, coup.second, capturedPiece, isPromotion);
            meilleurScore = std::max(meilleurScore, score);
            alpha = std::max(alpha, meilleurScore);
            if (score >= beta) {
                break;
            }
        }
        return meilleurScore;
    } else {
        int meilleurScore = std::numeric_limits<int>::max();
        for (const std::pair<int, int>& coup : possibleMoves) {
            Piece* capturedPiece = nullptr;
            bool isPromotion = board.isPromotionMove(coup.first, coup.second, m_color);
            int enPassantPos = -1;

            board.movePiece(coup.first, coup.second, (m_color == Color::WHITE) ? Color::BLACK : Color::WHITE, &capturedPiece, TypePieces::QUEEN,&enPassantPos);
            int score = minimax(board, profondeur - 1, true, alpha, beta);
            board.undoMove(coup.first, coup.second, capturedPiece, isPromotion);
            meilleurScore = std::min(meilleurScore, score);
            beta = std::min(beta, meilleurScore);
            if (score <= alpha) {
                break;
            }
        }
        return meilleurScore;
    }
}



