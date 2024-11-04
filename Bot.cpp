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
//@TODO L'ordering+nouveau système d'évaluation = suppression de pions et ralentissement (2x moins de noeud à évaluer mais 2,5x fois moins de nodes/s)


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
    int profondeur_max = 6;

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
    /*std::ranges::stable_sort(possibleMoves.begin(), possibleMoves.end(), std::greater<>{}, [&](const std::pair<int, int>& move) {
        return board.evaluateMove(move, m_color);
    });*/

    for (const std::pair<int, int>& coup : possibleMoves) {
        uint64_t zobristHash = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false);

        int score = evaluateMoveWithMinimax(board, profondeur_max, true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), coup, m_color);

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

    Color currentColor = estMaximisant ? m_color : (m_color == Color::WHITE ? Color::BLACK : Color::WHITE);
    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(currentColor);

    /*std::ranges::stable_sort(possibleMoves.begin(), possibleMoves.end(), std::greater<>{}, [&](const std::pair<int, int>& move) {
        return board.evaluateMove(move, m_color);
    });*/


    int meilleurScore = estMaximisant ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    for (const std::pair<int, int>& coup : possibleMoves) {
        int score = evaluateMoveWithMinimax(board, profondeur, estMaximisant, alpha, beta, coup, currentColor);

        if (estMaximisant) {
            meilleurScore = std::max(meilleurScore, score);
            alpha = std::max(alpha, meilleurScore);
            if (meilleurScore >= beta) {
                transpositionTable[zobristHash] = {profondeur, meilleurScore, ALPHA_CUT};
                return meilleurScore;
            }
        }
        else {
            meilleurScore = std::min(meilleurScore, score);
            beta = std::min(beta, meilleurScore);
            if (meilleurScore <= alpha) {
                transpositionTable[zobristHash] = {profondeur, meilleurScore, BETA_CUT};
                return meilleurScore;
            }
        }
    }

    transpositionTable[zobristHash] = {profondeur, meilleurScore, EXACT};
    return meilleurScore;
}


int Bot::evaluateMoveWithMinimax(Board& board, int profondeur, bool estMaximisant, int alpha, int beta, const std::pair<int, int>& move, Color currentColor) {
    Piece* capturedPiece = nullptr;
    bool isPromotion = board.isPromotionMove(move.first, move.second, currentColor);
    int enPassantPos = -1;
    board.movePiece(move.first, move.second, currentColor, &capturedPiece, TypePieces::QUEEN, &enPassantPos);
    int score = minimax(board, profondeur - 1, !estMaximisant, alpha, beta);
    board.undoMove(move.first, move.second, capturedPiece, isPromotion);
    return score;
}




