//
// Created by Peter on 02/10/2024.
//

#include "Bot.h"
#include "Zobrist.h"
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <chrono>

//@TODO Vérification de l'implémentation des table de transposition (docs sur le discord)
//@TODO Ajout des opération XOR pour le changement des hash avec Zobrist plutôt que tout recalculer (docs sur le discord)


/* Actuellement implémenter
- Minimax (parcours en profondeur)
- Alpha beta pruning
- Ordering node (pour optimiser le alpha beta)
- Table de transpositions (Pas optimiser mais fais déjà gagner des perf)
*/
int Bot::nodeCount = 0;

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

void Bot::choisir_meilleur_coup(Board& board, int profondeur_max, std::pair<int, int>& meilleurCoup) {
    int meilleurScore = std::numeric_limits<int>::min();
    meilleurCoup = { -1, -1 };
    Zobrist::initZobrist();

    nodeCount = 0;
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(m_color);
    std::sort(possibleMoves.begin(), possibleMoves.end(), [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return board.evaluateMove(a, m_color) > board.evaluateMove(b, m_color);
    });

    for (const std::pair<int, int>& coup : possibleMoves) {
        Piece* capturedPiece = nullptr;
        bool isPromotion = board.isPromotionMove(coup.first, coup.second, m_color);
        int enPassantPos = -1;
        uint64_t zobristHash = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false);

        board.movePiece(coup.first, coup.second, m_color, &capturedPiece, TypePieces::QUEEN,&enPassantPos);
        int score = minimax(board, profondeur_max - 1, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        board.undoMove(coup.first, coup.second, capturedPiece, isPromotion);
        transpositionTable[zobristHash] = {profondeur_max, score, 0};
        if (score > meilleurScore) {
            meilleurScore = score;
            meilleurCoup = coup;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Nodes evaluated: " << nodeCount << ", Time taken: " << duration.count() << " ms\n";
    std::cout << "Nodes/s: "<<nodeCount/(duration.count()/1000)<<"\n";
}

int Bot::minimax(Board& board, int profondeur, bool estMaximisant, int alpha, int beta) {
    nodeCount++;
    if (profondeur == 0) {
        return board.evaluate(m_color);
    }

    uint64_t zobristHash = Zobrist::computeZobristHash(board.getBoardStateAsVector(), m_color == Color::BLACK);

    if (transpositionTable.find(zobristHash) != transpositionTable.end()) {
        return transpositionTable[zobristHash].score;
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
                transpositionTable[zobristHash] = {profondeur, meilleurScore, ALPHA_CUT};
                break;
            }
        }
        transpositionTable[zobristHash] = {profondeur, meilleurScore, EXACT};
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
                transpositionTable[zobristHash] = {profondeur, meilleurScore, BETA_CUT};
                break;
            }
        }
        transpositionTable[zobristHash] = {profondeur, meilleurScore, EXACT};
        return meilleurScore;
    }
}



