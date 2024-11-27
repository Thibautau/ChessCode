//
// Created by Peter on 02/10/2024.
//

#include "Bot.h"
#include "Zobrist.h"
#include <cstdlib>
#include <math.h>
#include <limits>
#include <algorithm>
#include <chrono>
#include <fstream>

#include "MainChessGame.h"

//@TODO Vérification de l'implémentation des table de transposition (docs sur le discord)
//@TODO Ajout des opération XOR pour le changement des hash avec Zobrist plutôt que tout recalculer (docs sur le discord)
//@TODO L'ordering+nouveau système d'évaluation = suppression de pions et ralentissement (2x moins de noeud à évaluer mais 2,5x fois moins de nodes/s)
//@TODO Enlever les fonctions de debug (si inutile)


/* Actuellement implémenter
- Minimax (parcours en profondeur)
- Alpha beta pruning
- Ordering node (pour optimiser le alpha beta)
- Table de transpositions (Pas optimiser mais fais déjà gagner des perf)
*/
int Bot::nodeCount = 0;
int Bot::uniqueNodeIterated = 0;

Bot::Bot(Color color) : m_color(color)
{
    m_logFile = new LogFile("Bot_Evaluation_Log.txt", false);
}

Color Bot::getPlayerColor() const {
    return m_color;
}

void Bot::setPlayerColor(Color color) {
    m_color = color;
}

void Bot::play(Board& board, int& start, int& end) {
    std::pair<int, int> meilleurCoup;
    int profondeur_max = 6;

    choisir_meilleur_coup(board, profondeur_max, meilleurCoup);

    //m_logFile->clear();
    m_logFile->close();
    start = meilleurCoup.first;
    end = meilleurCoup.second;
}

void Bot::playWithDepth(Board& board, int& start, int& end, int depth, char& promotion) {
    std::pair<int, int> meilleurCoup;
    choisir_meilleur_coup(board, depth, meilleurCoup,&promotion);

    //m_logFile->clear();
    m_logFile->close();
    start = meilleurCoup.first;
    end = meilleurCoup.second;
    std::cout << "coup: " << meilleurCoup.first << "  " <<meilleurCoup.second << "\n";
    std::cout << "promotion: " << promotion << "\n";
}

std::string indexToPosition(int pos) {
    int row = pos / 8;
    int col = pos % 8;

    char columnChar = 'a' + col;
    char rowChar = '1' + row;

    return std::string{columnChar} + rowChar;
}

void debugPrintMove(int start, int end) {
    std::cout << indexToPosition(start) << " => " << indexToPosition(end) << std::endl;
}

void afficherPossibleMoves(const std::vector<std::pair<int, int>>& possibleMoves) {
    for (const auto& move : possibleMoves) {
        debugPrintMove(move.first, move.second);
    }
}

void comparerVecteurs(const std::vector<std::pair<int, int>>& vec1, const std::vector<std::pair<int, int>>& vec2) {
    size_t taille_min = std::min(vec1.size(), vec2.size());

    for (size_t i = 0; i < taille_min; ++i)
    {
        if(vec1[i].first != vec2[i].first || vec1[i].second != vec2[i].second)
        {
            std::cout << "(" << indexToPosition(vec1[i].first) << " => " << indexToPosition(vec1[i].second) << ") != "
                      << "(" << indexToPosition(vec2[i].first) << " => " << indexToPosition(vec2[i].second) << ")\n";
        }
    }

    if (vec1.size() != vec2.size())
    {
        std::cout << "Les vecteurs ont des tailles différentes.\n" << std::endl;
    }
    else
    {
        std::cout << "Les vecteurs ont exactement la même taille.\n" << std::endl;
    }
}


void Bot::clearFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc); // Ouvre le fichier en mode 'trunc' (truncate)
    if (!file.is_open()) {
        std::cerr << "Erreur d'ouverture du fichier pour suppression du contenu." << std::endl;
    }
    // Si le fichier est ouvert en mode 'trunc', il est automatiquement vidé.
    file.close();
}

void Bot::choisir_meilleur_coup(Board& board, int profondeur_max, std::pair<int, int>& meilleurCoup, char* bestPromotion) {
    int meilleurScore = std::numeric_limits<int>::min();
    m_logFile->clear();
    //clearFile("../debug_log.txt");
    meilleurCoup = { -1, -1 };
    std::pair<int, int> previousBestMove = { -1, -1 };
    Zobrist::initZobrist();

    nodeCount = 0;
    uniqueNodeIterated = 0;
    char promotion = '\0';
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(m_color);

    // Vérifiez si des coups sont possibles
    if (possibleMoves.empty()) {
        return;
    }

    // Trier les mouvements en utilisant l'évaluation optimisée
    std::ranges::stable_sort(possibleMoves.begin(), possibleMoves.end(), [&](const std::pair<int, int>& move1, const std::pair<int, int>& move2) {
        return board.evaluateMove(move1, m_color) > board.evaluateMove(move2, m_color);
    });

    for (const std::pair<int, int>& coup : possibleMoves) {
        uint64_t zobristHash = Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), board.getEnPassantState());

        if (board.isPromotionMove(coup.first, coup.second, m_color)) {
            for (char promoType : {'q', 'r', 'b', 'n'}) {
                promotion = promoType;
                int score = evaluateMoveWithMinimax(board, profondeur_max, true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), coup, m_color, promotion);
                // Mettre à jour la table de transposition uniquement si le coup est trouvé
                if (transpositionTable.find(zobristHash) == transpositionTable.end()) {
                    transpositionTable[zobristHash] = {profondeur_max, score, 0};
                }
                // Si aucun bon coup n'a été trouvé, c'est qu'on est échec et mat mais le coup qu'on évalue est peut etre notre seule possibilité
                if (score == std::numeric_limits<int>::min() && meilleurScore == std::numeric_limits<int>::min())
                {
                    previousBestMove = coup;
                    if (bestPromotion) *bestPromotion = promotion;
                }

                if (score > meilleurScore) {
                    meilleurScore = score;
                    previousBestMove = coup;
                    if (bestPromotion) *bestPromotion = promotion;
                }
            }
        }
        else {
            int score = evaluateMoveWithMinimax(board, profondeur_max, true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), coup, m_color, promotion);
            // Mettre à jour la table de transposition uniquement si le coup est trouvé
            if (transpositionTable.find(zobristHash) == transpositionTable.end()) {
                transpositionTable[zobristHash] = {profondeur_max, score, 0};
            }
            // Si aucun bon coup n'a été trouvé, c'est qu'on est échec et mat mais le coup qu'on évalue est peut etre notre seule possibilité
            if (score == std::numeric_limits<int>::min() && meilleurScore == std::numeric_limits<int>::min())
            {
                previousBestMove = coup;
                if (bestPromotion) *bestPromotion = promotion;
            }
            if (score > meilleurScore) {
                meilleurScore = score;
                previousBestMove = coup;
                if (bestPromotion) *bestPromotion = promotion;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    // Si aucun meilleur coup n'a été trouvé, utilisez le coup de la profondeur précédente
    if (meilleurCoup == std::pair<int, int>{ -1, -1 }) {
        meilleurCoup = previousBestMove;
    }

    transpositionTable.clear();

    /*std::cout << "Nodes evaluated: " << nodeCount << ", Time taken: " << duration.count() << " ms\n";
    std::cout << "Nodes/s: "<<nodeCount/(duration.count()/1000)<<"\n";*/
}



int Bot::alphaBetaWithMemoryv2(Board& board, int depth, int alpha, int beta, bool estMaximisant, char &bestPromotion) {
    nodeCount++;
    uint64_t zobristHash = Zobrist::computeZobristHash(board.getBoardStateAsVector(), m_color == Color::BLACK, board.getCastlingStateAsVector(), board.getEnPassantState());

    // Initialisation des variables
    int old_best = -1, curr_best = -1;
    int g = estMaximisant ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    bestPromotion = '\0';

    // Recherche dans la table de transposition
    if (transpositionTable.find(zobristHash) != transpositionTable.end()) {
        TranspositionTableEntry& entry = transpositionTable[zobristHash];
        old_best = entry.bestMoveIndex;

        // Si profondeur correspondante
        if (entry.depth == depth) {
            if (entry.lowerBound == entry.upperBound) return entry.lowerBound;
            if (entry.lowerBound >= beta) return entry.lowerBound;
            if (entry.upperBound <= alpha) return entry.upperBound;

            alpha = std::max(alpha, entry.lowerBound);
            beta = std::min(beta, entry.upperBound);
        }
    }

    // Cas de base : profondeur 0
    if (depth == 0) {
        int evaluation = board.evaluate(m_color);
        transpositionTable[zobristHash] = {depth, evaluation, evaluation, EXACT};
        return evaluation;
    }

    // Liste des coups et réordonnancement
    Color currentColor = estMaximisant ? m_color : (m_color == Color::WHITE ? Color::BLACK : Color::WHITE);
    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(currentColor);

    if (old_best != -1 && old_best < possibleMoves.size()) {
        std::rotate(possibleMoves.begin(), possibleMoves.begin() + old_best, possibleMoves.begin() + old_best + 1);
    }

    for (size_t j = 0; j < possibleMoves.size(); ++j) {
        const std::pair<int, int> move = possibleMoves[j];
        char tempPromotion = '\0';

        // Évaluation du coup
        if (board.isPromotionMove(move.first, move.second, currentColor)) {
            for (char promoType : {'q', 'n', 'b', 'r'}) {
                tempPromotion = promoType;
                int t = evaluateMoveWithMinimax(board, depth, estMaximisant, alpha, beta, move, currentColor, tempPromotion);

                if (estMaximisant && t > g) g = t, curr_best = j, bestPromotion = tempPromotion;
                else if (!estMaximisant && t < g) g = t, curr_best = j, bestPromotion = tempPromotion;

                if (estMaximisant) alpha = std::max(alpha, g);
                else beta = std::min(beta, g);

                if (alpha >= beta) break;
            }
        }
        else {
            int t = evaluateMoveWithMinimax(board, depth, estMaximisant, alpha, beta, move, currentColor, tempPromotion);

            if (estMaximisant && t > g) g = t, curr_best = j, bestPromotion = '\0';
            else if (!estMaximisant && t < g) g = t, curr_best = j, bestPromotion = '\0';

            if (estMaximisant) alpha = std::max(alpha, g);
            else beta = std::min(beta, g);

            if (alpha >= beta) break;
        }
    }

    // Mise à jour de la table de transposition
    if (curr_best != -1) transpositionTable[zobristHash].bestMoveIndex = curr_best;
    if (g <= alpha) transpositionTable[zobristHash] = {depth, transpositionTable[zobristHash].lowerBound, g, UPPER};
    else if (g >= beta) transpositionTable[zobristHash] = {depth, g, transpositionTable[zobristHash].upperBound, LOWER};
    else transpositionTable[zobristHash] = {depth, g, g, EXACT};

    return g;
}




int Bot::alphaBetaWithMemory(Board& board, int depth, int alpha, int beta, bool estMaximisant, char &bestPromotion) {
    nodeCount++;
    uint64_t zobristHash = Zobrist::computeZobristHash(board.getBoardStateAsVector(), m_color == Color::BLACK, board.getCastlingStateAsVector(), board.getEnPassantState());

    // Recherche dans la table de transposition
    if (transpositionTable.find(zobristHash) != transpositionTable.end()) {
        const TranspositionTableEntry& entry = transpositionTable[zobristHash];
        if (entry.depth == depth) {
            if (entry.score >= beta) return entry.score;
            if (entry.score <= alpha) return entry.score;
            alpha = std::max(alpha, entry.score);
            beta = std::min(beta, entry.score);
        }
    }

    // Cas de base : profondeur 0
    if (depth == 0) {
        int evaluation = board.evaluate(m_color);  // Évaluation de la position pour le joueur courant
        transpositionTable[zobristHash] = {depth, evaluation, EXACT};
        return evaluation;
    }

    // Détermination de la couleur à maximiser ou minimiser
    Color currentColor = estMaximisant ? m_color : (m_color == Color::WHITE ? Color::BLACK : Color::WHITE);
    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(currentColor);

    // Tri des coups en fonction de leur évaluation
    std::ranges::stable_sort(possibleMoves.begin(), possibleMoves.end(), std::greater<>{}, [&](const std::pair<int, int>& move) {
        return board.evaluateMove(move, m_color);
    });

    if (possibleMoves.empty()) return estMaximisant ? std::numeric_limits<int>::min(): std::numeric_limits<int>::max();

    int bestScore = estMaximisant ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    char promotion = '\0';
    bestPromotion = '\0';

    for (const auto& move : possibleMoves) {
        if (board.isPromotionMove(move.first, move.second, currentColor)) {
            // Essayer chaque promotion
            for (char promoType : {'q', 'n', 'b', 'r'}) {
                promotion = promoType;
                int score = evaluateMoveWithMinimax(board, depth, estMaximisant, alpha, beta, move, currentColor, promotion);

                if (estMaximisant) {
                    if (score > bestScore) bestScore = score, bestPromotion = promoType;
                    alpha = std::max(alpha, bestScore);
                    if (bestScore >= beta) break;
                }
                else {
                    if (score < bestScore) bestScore = score, bestPromotion = promoType;
                    beta = std::min(beta, bestScore);
                    if (bestScore <= alpha) break;
                }
            }
        }
        else {
            int score = evaluateMoveWithMinimax(board, depth, estMaximisant, alpha, beta, move, currentColor, promotion);
            if (estMaximisant) {
                if (score > bestScore) bestScore = score, bestPromotion = '\0';
                alpha = std::max(alpha, bestScore);
                if (bestScore >= beta) break;
            } else {
                if (score < bestScore) bestScore = score, bestPromotion = '\0';
                beta = std::min(beta, bestScore);
                if (bestScore <= alpha) break;
            }
        }
    }

    // Sauvegarde du résultat dans la table de transposition
    transpositionTable[zobristHash] = {depth, bestScore, EXACT};

    return bestScore;
}

int Bot::alphaBetaBasic(Board& board, int depth, int alpha, int beta, bool estMaximisant, char &bestPromotion) {
    nodeCount++;

    // Cas de base : profondeur 0
    if (depth == 0) {
        // Evaluation de la position à profondeur 0
        int evaluation = board.evaluateTest(m_color);
        return evaluation;
    }

    // Ouvrir le fichier de log une seule fois avant la boucle principale
    /*static std::ofstream logFile("../debug_log.txt", std::ios::app); // Ouvrir en mode append
    if (!logFile.is_open()) {
        std::cerr << "Erreur d'ouverture du fichier de log" << std::endl;
        return 0;
    }*/

    if (depth == 3) {
        //logFile << "Plateau à la profondeur " << depth << " :\n" << board.getBoardAsString() << "\n";
        std::string logMessage = "Plateau à la profondeur " + std::to_string(depth) + " :\n" + board.getBoardAsString() + "\n";
        m_logFile->logInfo(logMessage);
    }

    // Détermination de la couleur à maximiser ou minimiser
    Color currentColor = estMaximisant ? m_color : (m_color == Color::WHITE ? Color::BLACK : Color::WHITE);
    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(currentColor);

    // Tri des coups en fonction de leur évaluation
    std::ranges::stable_sort(possibleMoves.begin(), possibleMoves.end(), std::greater<>{}, [&](const std::pair<int, int>& move) {
        return board.evaluateMove(move, m_color);
    });

    if (possibleMoves.empty()) return estMaximisant ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    int bestScore = estMaximisant ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    char promotion = '\0';
    bestPromotion = '\0';

    // Boucle sur tous les coups possibles
    for (const auto& move : possibleMoves) {
        if (board.isPromotionMove(move.first, move.second, currentColor)) {
            // Essayer chaque promotion
            for (char promoType : {'q', 'n', 'b', 'r'}) {
                promotion = promoType;
                int score = evaluateMoveWithMinimax(board, depth, estMaximisant, alpha, beta, move, currentColor, promotion);

                std::string logMessage = "Move: " + indexToPosition( move.first) + " -> " + indexToPosition(move.second)
                        + " Promotion: " + promoType
                        + " Maximisant: " + (estMaximisant ? "true" : "false")
                        + " Score: " + std::to_string(score)
                        + " Alpha: " + std::to_string(alpha)
                        + " Beta: " + std::to_string(beta)
                        + " Depth: " + std::to_string(depth) + "\n";
                m_logFile->logInfo(logMessage);


                if (estMaximisant) {
                    if (score > bestScore) bestScore = score, bestPromotion = promoType;
                    alpha = std::max(alpha, bestScore);
                    if (bestScore >= beta) break;
                }
                else {
                    if (score < bestScore) bestScore = score, bestPromotion = promoType;
                    beta = std::min(beta, bestScore);
                    if (bestScore <= alpha) break;
                }


            }
        } else {
            // Evaluation du coup sans promotion
            int score = evaluateMoveWithMinimax(board, depth, estMaximisant, alpha, beta, move, currentColor, promotion);

            // Log des informations de mouvement
            std::string logMessage = "Move: " + indexToPosition( move.first) + " -> " + indexToPosition(move.second)
                        + " Maximisant: " + (estMaximisant ? "true" : "false")
                        + " Score: " + std::to_string(score)
                        + " Alpha: " + std::to_string(alpha)
                        + " Beta: " + std::to_string(beta)
                        + " Depth: " + std::to_string(depth) + "\n";
            m_logFile->logInfo(logMessage);


            if (estMaximisant) {
                if (score > bestScore) bestScore = score, bestPromotion = '\0';
                alpha = std::max(alpha, bestScore);
                if (bestScore >= beta) break;
            } else {
                if (score < bestScore) bestScore = score, bestPromotion = '\0';
                beta = std::min(beta, bestScore);
                if (bestScore <= alpha) break;
            }
        }
    }
    return bestScore;
}


int Bot::evaluateMoveWithMinimax(Board& board, int profondeur, bool estMaximisant, int alpha, int beta, const std::pair<int, int>& move, Color currentColor, char& promotion) {
    Piece* capturedPiece = nullptr;
    bool isPromotion = board.isPromotionMove(move.first, move.second, currentColor);
    int enPassantPos = -1;
    board.movePiece(move.first, move.second, currentColor, &capturedPiece, Piece::charToPieceType(promotion), &enPassantPos);
    int score = alphaBetaBasic(board, profondeur - 1, alpha, beta, !estMaximisant, promotion);
    board.undoMove(move.first, move.second, capturedPiece, isPromotion);
    return score;
}







