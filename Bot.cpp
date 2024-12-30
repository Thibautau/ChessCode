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
    m_logFile = new LogFile("Bot_Evaluation_Log.txt", true);
    m_openingBook = new OpeningBook();
    m_openingBook->getBookData("../OpeningBook/Books/Player1.bin");
    m_openingBook->getBookData("../OpeningBook/Books/Player2.bin");
    m_openingBook->getBookData("../OpeningBook/Books/Player3.bin");
    m_openingBook->getBookData("../OpeningBook/Books/Player4.bin");
}

Color Bot::getPlayerColor() const {
    return m_color;
}

void Bot::setPlayerColor(Color color) {
    m_color = color;
}

void Bot::play(Board& board, int& start, int& end) {
    std::pair<int, int> meilleurCoup;
    int profondeur_max = 4;
    m_max_depth = profondeur_max;

    uint64_t boardHash = Zobrist::computeZobristHash(m_color == Color::WHITE, board);
    std::string sBestmove;
    bool bIsThereBestMoveInOpeningBook = m_openingBook->getBestMoveForHash(sBestmove, boardHash);
    if(bIsThereBestMoveInOpeningBook && ! sBestmove.empty())
    {
        start = Board::convertToPosition(sBestmove[0], sBestmove[1]);
        end = Board::convertToPosition(sBestmove[2], sBestmove[3]);

        return;
    }
    m_max_depth_Quiescence = m_max_depth+m_diff_between_depth;

    choisir_meilleur_coupv2(board, m_max_depth_Quiescence, meilleurCoup);

    //m_logFile->clear();
    m_logFile->close();
    start = meilleurCoup.first;
    end = meilleurCoup.second;
}

void Bot::playWithDepth(Board& board, int& start, int& end, int depth, char& promotion) {
    std::pair<int, int> meilleurCoup;
    uint64_t boardHash = Zobrist::computeZobristHash(m_color == Color::WHITE, board);
    std::string sBestmove;
    bool bIsThereBestMoveInOpeningBook = m_openingBook->getBestMoveForHash(sBestmove, boardHash);
    if(bIsThereBestMoveInOpeningBook && ! sBestmove.empty())
    {
        start = Board::convertToPosition(sBestmove[0], sBestmove[1]);
        end = Board::convertToPosition(sBestmove[2], sBestmove[3]);

        if(sBestmove.length() == 5) //promootion
        {
            promotion = sBestmove[4];
        }
        return;
    }

    m_max_depth = depth;
    m_max_depth_Quiescence = m_max_depth+m_diff_between_depth;

    choisir_meilleur_coupv2(board, m_max_depth_Quiescence, meilleurCoup,&promotion);

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



int Bot::Quiescence(Board& board, int depth, int alpha, int beta, bool estMaximisant,char &bestPromotion) {
    uint64_t zobristHash = board.getZobristHash();
    // Recherche dans la table de transposition
    auto it = transpositionTable.find(zobristHash);
    if (it != transpositionTable.end()) {
        const TranspositionTableEntry& entry = it->second;
        if (entry.depth >= depth) {
            if (entry.flag == EXACT) return entry.score;
            if (entry.flag == LOWERBOUND && entry.score >= beta) return entry.score;
            if (entry.flag == UPPERBOUND && entry.score <= alpha) return entry.score;
        }
    }

    int standPat = board.evaluateTest(m_color);

    if (standPat >= beta) {return standPat;}
    if (standPat > alpha) {alpha = standPat;}

    //Limite = -Max_Depth_Quiescence
    if (depth <= -2) {return standPat;}

    // Liste des coups qui capturent uniquement
    std::pair<int, int> list_move[112];
    int moveCount = 0;
    board.getAllPiecesEatableByAColor(m_color, list_move, moveCount);

    for(std::pair<int, int> move : list_move)
    {
        if(move == std::make_pair(53, 31))
        {
            int a = 2;
        }
    }

    // Couleur actuelle
    Color currentColor = estMaximisant ? m_color : (m_color == Color::WHITE ? Color::BLACK : Color::WHITE);

    //Rénitialisation des variables
    int bestScore = estMaximisant ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    Piece* capturedPiece = nullptr;
    for (int i = 0; i < moveCount; ++i) {
        const std::pair<int, int>& move = list_move[i];

        //Gestion des promotions
        const char* promotionTypes;
        size_t promotionCount;

        if (board.isPromotionMove(move.first, move.second, currentColor)) {
            promotionTypes = PROMOTION_TYPES;
            promotionCount = std::size(PROMOTION_TYPES);
        }
        else {
            promotionTypes = NO_PROMOTION;
            promotionCount = std::size(NO_PROMOTION);
        }

        // Sauvegarde de l'état du plateau
        uint64_t originalHash = board.getZobristHash();
        int enPassantPos = board.getEnPassantPosition();
        int itabCastlingRights[4];
        board.getCastlingStateAsTableau(itabCastlingRights);
        bool bisWhiteKingCheked = board.isWhiteKingCheck();
        bool bisBlackKingCheked = board.isBlackKingCheck();
        int iWhiteKingPosition = board.getKingPosition(Color::WHITE);
        int iBlackKingPosition = board.getKingPosition(Color::BLACK);

        for (size_t i = 0; i < promotionCount; ++i) {
            char promoType = promotionTypes[i];

            // Jouer le coup
            bool bCouldMove = board.movePiece(move.first, move.second, currentColor, &capturedPiece, Piece::charToPieceType(promoType));
            if(! bCouldMove)
            {
                std::cerr << "ERROR IN QUIESCENCE, illegal move :(" << std::to_string(move.first) << "-" << std::to_string(move.second) << ")\n" << board.getBoardAsString() << std::endl;
                std::string logMessage2 = "ERROR IN QUIESCENCE, illegal move :\n" + board.getBoardAsString() + "\n";
                m_logFile->logInfo(logMessage2 + " Move:" + std::to_string(move.first) + "-" + std::to_string(move.second));
            }

            // Calcul du score (Négamax)
            int score = -Quiescence(board, depth - 1, -alpha, -beta, !estMaximisant, bestPromotion);

            // Mise à jour du hash pour le coup
            calculateZobristHashForMove(board, move, m_color, promoType, promoType != '\0', zobristHash, capturedPiece);
            board.setZobristHash(zobristHash);

            // Annuler le coup
            bool bCouldUndo = board.undoMove(move.first, move.second, capturedPiece, promoType != '\0', enPassantPos, itabCastlingRights, bisWhiteKingCheked, bisBlackKingCheked, iWhiteKingPosition, iBlackKingPosition);
            board.setZobristHash(originalHash);
            if(! bCouldUndo)
            {
                std::cerr << "ERROR IN QUIESCENCE, Undomove :(" << std::to_string(move.first) << "-" << std::to_string(move.second) << ")\n" << board.getBoardAsString() << std::endl;
                std::string logMessage2 = "ERROR IN QUIESCENCE, undoMove :\n" + board.getBoardAsString() + "\n";
                m_logFile->logInfo(logMessage2 + " Move:" + std::to_string(move.first) + "-" + std::to_string(move.second));
            }

            // Mise à jour des scores et alpha/beta
            if (estMaximisant) {
                if (score > bestScore) {
                    bestScore = score;
                    bestPromotion = promoType;
                }
                alpha = std::max(alpha, score);
                if (alpha >= beta) {
                    transpositionTable[zobristHash] = {depth, bestScore, LOWERBOUND};
                    return bestScore;
                }
            }
            else {
                if (score < bestScore) {
                    bestScore = score;
                    bestPromotion = promoType;
                }
                beta = std::min(beta, score);
                if (beta <= alpha) {
                    transpositionTable[zobristHash] = {depth, bestScore, UPPERBOUND};
                    return bestScore;
                }
            }
        }
    }

    // Sauvegarde du résultat dans la table de transposition
    transpositionTable[zobristHash] = {depth, bestScore, EXACT};

    return alpha;
}


void Bot::choisir_meilleur_coupv2(Board& board, int profondeur_max, std::pair<int, int>& meilleurCoup, char* bestPromotion) {
    // Initialisation des variables
    int meilleurScore = std::numeric_limits<int>::min();
    m_logFile->clear();
    meilleurCoup = { -1, -1 };
    std::pair<int, int> previousBestMove = { -1, -1 };

    // Initialisation de Zobrist et du hash de la position
    board.setZobristHash(Zobrist::computeZobristHash(board.getBoardStateAsVector(), false, board.getCastlingStateAsVector(), board.getEnPassantState()));

    // Variables pour récupérer les stats et informations diverses
    nodeCount = 0;
    uniqueNodeIterated = 0;
    char promotion = '\0';
    auto start = std::chrono::high_resolution_clock::now();

    // Obtenir les mouvements possibles
    std::vector<std::pair<int, int>> possibleMoves = board.listOfPossibleMoves(m_color);
    if (possibleMoves.empty()) {
        return;
    }

    // Trier les mouvements en utilisant l'évaluation optimisée
    std::ranges::stable_sort(possibleMoves.begin(), possibleMoves.end(), [&](const std::pair<int, int>& move1, const std::pair<int, int>& move2) {
        return board.evaluateMove(move1, m_color) > board.evaluateMove(move2, m_color);
    });

    // Variables de promotion
    const char* promotionTypes = nullptr;
    size_t promotionCount = 0;

    // Évaluer chaque coup possible
    for (const std::pair<int, int>& move : possibleMoves) {
        uint64_t zobristHash = board.getZobristHash();
        bool isPromotion = board.isPromotionMove(move.first, move.second, m_color);

        if (isPromotion) {
            promotionTypes = PROMOTION_TYPES;
            promotionCount = std::size(PROMOTION_TYPES);
        } else {
            promotionTypes = NO_PROMOTION;
            promotionCount = std::size(NO_PROMOTION);
        }

        for (size_t i = 0; i < promotionCount; ++i) {
            char promoType = promotionTypes[i];
            uint64_t originalHash = board.getZobristHash();
            Piece* capturedPiece = nullptr;

            //Etat actuel du plateau
            int enPassantPos = board.getEnPassantPosition();
            int itabCastlingRights[4] = { -1, -1, -1, -1 };
            board.getCastlingStateAsTableau(itabCastlingRights);
            bool bisWhiteKingCheked = board.isWhiteKingCheck();
            bool bisBlackKingCheked = board.isBlackKingCheck();
            int iWhiteKingPosition = board.getKingPosition(Color::WHITE);
            int iBlackKingPosition = board.getKingPosition(Color::BLACK);

            if(move.first == 10 && move.second == 3) {
                int a = 2;
            }

            // Jouer le coup
            bool bCanMove = board.movePiece(move.first, move.second, m_color, &capturedPiece, Piece::charToPieceType(promoType));
            std::string logMessage = "Plateau à la profondeur :\n" + board.getBoardAsString() + "\n";
            m_logFile->logInfo(logMessage + " Move:" + std::to_string(move.first) + "-" + std::to_string(move.second));
            if(! bCanMove)
            {
                //board.displayBoard();
                board.displayBoard();
                std::cerr << "Error: The previous move was (" << board.getPreviousMoveInitialPosition() << "-" << board.getPreviousMoveTargetPosition() << ")" << std::endl;
                std::cerr << "But the The acutal move is (" << move.first << "-" << move.second << ")" << std::endl;
                std::string logMessage2 = "ERRORPlateau à la profondeur :\n" + board.getBoardAsString() + "\n";
                m_logFile->logInfo(logMessage2 + " Move:" + std::to_string(move.first) + "-" + std::to_string(move.second));
                //std::cerr << "Error: Impossible move (" << move.first << " " << move.second << "). in choisir_meilleur_coupV2." << std::endl;
            }

            int iPreviousInitialPosition = move.first;
            int iPreviousTargetPosition = move.second;
            board.setPreviousMoveInitialPosition(iPreviousInitialPosition);
            board.setPreviousMoveTargetPosition(iPreviousTargetPosition);

            // Mise à jour du hash pour le coup
            calculateZobristHashForMove(board, move, m_color, promoType, promoType != '\0', zobristHash, capturedPiece);
            board.setZobristHash(zobristHash);

            if(move.first == 10 && move.second == 3) {
                int a = 2;
            }
            int score = alphaBetaWithMemory(board, profondeur_max-1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false, promotion);

            // Annuler le coup
            bool bCanUndo = board.undoMove(move.first, move.second, capturedPiece, promoType != '\0', enPassantPos, itabCastlingRights, bisWhiteKingCheked, bisBlackKingCheked, iWhiteKingPosition, iBlackKingPosition);
            if(! bCanUndo) {
                board.displayBoard();
                std::cerr << "Error: UNDOMOVE FAILED" << move.first << " " << move.second << "). in choisirMeilleurCoupV2." << std::endl;
            }
            board.setPreviousMoveInitialPosition(iPreviousInitialPosition);
            board.setPreviousMoveTargetPosition(iPreviousTargetPosition);
            board.setZobristHash(originalHash);

            // Vérification de la table de transposition avant d'ajouter
            if (transpositionTable.find(zobristHash) == transpositionTable.end()) {
                transpositionTable[zobristHash] = {profondeur_max, score, 0};
            }

            // Gérer les cas où aucun bon coup n'est trouvé
            if (score == std::numeric_limits<int>::min() && meilleurScore == std::numeric_limits<int>::min()) {
                previousBestMove = move;
                if (bestPromotion) *bestPromotion = promoType; //promotion
            }

            // Mettre à jour le meilleur coup
            if (score > meilleurScore) {
                meilleurScore = score;
                previousBestMove = move;
                if (bestPromotion) *bestPromotion = promoType; //promotion
            }
        }
    }

    // Temps d'exécution
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    // Si aucun meilleur coup n'a été trouvé, utilisez le coup de la profondeur précédente
    if (meilleurCoup == std::pair<int, int>{ -1, -1 }) {
        meilleurCoup = previousBestMove;
    }

    transpositionTable.clear();
}



int Bot::alphaBetaWithMemory(Board& board, int depth, int alpha, int beta, bool estMaximisant, char &bestPromotion) {
    nodeCount++;
    uint64_t zobristHash = board.getZobristHash();

    // Recherche dans la table de transposition
    auto it = transpositionTable.find(zobristHash);
    if (it != transpositionTable.end()) {
        const TranspositionTableEntry& entry = it->second;
        if (entry.depth >= depth) {
            if (entry.flag == EXACT) return entry.score;
            if (entry.flag == LOWERBOUND && entry.score >= beta) return entry.score;
            if (entry.flag == UPPERBOUND && entry.score <= alpha) return entry.score;
        }
    }

    // Cas de base : profondeur 0
    if (depth == 0) {
        int evaluation = board.evaluateTest(m_color);
        transpositionTable[zobristHash] = {depth, evaluation, EXACT};
        return evaluation;
    }

    // Détermination de la couleur à maximiser ou minimiser
    Color currentColor = estMaximisant ? m_color : (m_color == Color::WHITE ? Color::BLACK : Color::WHITE);
    std::pair<int, int> possibleMoves[128];
    int moveCount = 0;

    if(depth > m_diff_between_depth) {
        board.listOfPossibleMoves(currentColor, possibleMoves, moveCount);
    }
    else {
        board.getAllPiecesEatableByAColor(m_color, possibleMoves, moveCount);
    }


    // Tri des coups en fonction de leur évaluation
    std::ranges::stable_sort(possibleMoves, possibleMoves + moveCount, std::greater<>{}, [&](const std::pair<int, int>& move) {
        return board.evaluateMove(move, m_color);
    });

    //Mise en echec et mat ou PAT
    if (moveCount == 0) {
        return estMaximisant ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    }

    //Rénitialisation des variables
    int bestScore = estMaximisant ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();


    for (int i = 0; i < moveCount; ++i) {
        const std::pair<int, int>& move = possibleMoves[i];
        //Pour gérer les promotions (car pas gérer de base, les mouvements sont des pairs de int)
        const char* promotionTypes;
        size_t promotionCount;

        if (board.isPromotionMove(move.first, move.second, currentColor)) {
            promotionTypes = PROMOTION_TYPES;
            promotionCount = std::size(PROMOTION_TYPES);
        }
        else {
            promotionTypes = NO_PROMOTION;
            promotionCount = std::size(NO_PROMOTION);
        }

        for (size_t i = 0; i < promotionCount; ++i) {
            char promoType = promotionTypes[i];
            uint64_t originalHash = board.getZobristHash();
            Piece* capturedPiece = nullptr;

            //Etat actuel du plateau
            int enPassantPos = board.getEnPassantPosition();
            int itabCastlingRights[4] = { -1, -1, -1, -1 };
            board.getCastlingStateAsTableau(itabCastlingRights);
            bool bisWhiteKingCheked = board.isWhiteKingCheck();
            bool bisBlackKingCheked = board.isBlackKingCheck();
            int iWhiteKingPosition = board.getKingPosition(Color::WHITE);
            int iBlackKingPosition = board.getKingPosition(Color::BLACK);
            int iPreviousInitialPosition = board.getPreviousMoveInitialPosition();
            int iPreviousTargetPosition = board.getPreviousMoveTargetPosition();

            if(move.first == 0 && move.second == 3) {
                int a = 2;
            }if(move.first == 4 && move.second == 3) {
                int a = 2;
            }if(move.first == 18 && move.second == 3) {
                int a = 2;
            }if(move.first == 4 && move.second == 2) {
                int a = 2;
            }

            // Jouer le coup
            bool bCanMove = board.movePiece(move.first, move.second, currentColor, &capturedPiece, Piece::charToPieceType(promoType));
            std::string logMessage = "Plateau à la profondeur " + std::to_string(depth) + " :\n" + board.getBoardAsString() + "\n";
            m_logFile->logInfo(logMessage + " Move:" + std::to_string(move.first) + "-" + std::to_string(move.second));
            if(! bCanMove)
            {
                board.displayBoard();
                std::string logMessage2 = "Plateau à la profondeur :\n" + board.getBoardAsString() + "\n";
                m_logFile->logInfo(logMessage2 + " Move:" + std::to_string(move.first) + "-" + std::to_string(move.second));
                std::cerr << "Error: Impossible move (" << move.first << " " << move.second << "). in alphaBetaWithMemory." << std::endl;
            }
            // Mise à jour du hash pour le coup
            calculateZobristHashForMove(board, move, currentColor, promoType, promoType != '\0', zobristHash, capturedPiece);
            board.setZobristHash(zobristHash);

            // Appel récursif
            int score = alphaBetaWithMemory(board, depth - 1, alpha, beta, !estMaximisant, bestPromotion);

            // Annuler le coup
            bool bCanUndo = board.undoMove(move.first, move.second, capturedPiece, promoType != '\0', enPassantPos, itabCastlingRights, bisWhiteKingCheked, bisBlackKingCheked, iWhiteKingPosition, iBlackKingPosition);
            if(! bCanUndo) {
                std::cerr << "Error: UNDOMOVE FAILED" << move.first << " " << move.second << "). in alphaBetaWithMemory." << std::endl;
            }
            board.setPreviousMoveInitialPosition(iPreviousInitialPosition);
            board.setPreviousMoveTargetPosition(iPreviousTargetPosition);
            board.setZobristHash(originalHash);

            // Mise à jour des scores et alpha/beta
            if (estMaximisant) {
                if (score > bestScore) {
                    bestScore = score;
                    bestPromotion = promoType;
                }
                alpha = std::max(alpha, score);
                if (alpha >= beta) {
                    transpositionTable[zobristHash] = {depth, bestScore, LOWERBOUND};
                    return bestScore;
                }
            }
            else {
                if (score < bestScore) {
                    bestScore = score;
                    bestPromotion = promoType;
                }
                beta = std::min(beta, score);
                if (beta <= alpha) {
                    transpositionTable[zobristHash] = {depth, bestScore, UPPERBOUND};
                    return bestScore;
                }
            }

        }
    }

    // Sauvegarde du résultat dans la table de transposition
    transpositionTable[zobristHash] = {depth, bestScore, EXACT};

    return bestScore;
}

void Bot::calculateZobristHashForMove(Board& board, const std::pair<int, int>& move, Color currentColor, char promotionForMove, bool isPromotion, uint64_t& zobristHash, Piece* capturedPiece) {
    Piece* piece_depart = board.getPieceAt(move.second);
    Piece* piece_arrivee = capturedPiece;

    if(!piece_depart) {
        //board.displayBoard();
        std::cerr << "Warning: No piece found at start square (" << move.second << "). Check board state." << "(" << move.first << "-" << move.second << ")" << std::endl;
    }

    int itabCastlingRights[4];
    board.getCastlingRightsLostByMoving(itabCastlingRights);
    int enPassantState = board.getEnPassantState();

    // If it's at the black to play
    //if (currentColor != Color::BLACK) {
    //if (currentColor == Color::BLACK) {
        zobristHash ^= Zobrist::getHashForWhiteTurn();
    //}

    // In case of capture
    if (piece_arrivee) {
        zobristHash ^= Zobrist::getPieceHash(board.getIndexByPiece(piece_arrivee->getTypePiece(), piece_arrivee->getColor()), move.second);//Remove the piece captured
    }

    // XOR the start and end of the moved piece
    if(! isPromotion)
    {
        zobristHash ^= Zobrist::getPieceHash(board.getIndexByPiece(piece_depart->getTypePiece(), piece_depart->getColor()), move.first);//Remove the piece moving
        zobristHash ^= Zobrist::getPieceHash(board.getIndexByPiece(piece_depart->getTypePiece(), piece_depart->getColor()), move.second);//Place the piece moving
    }
    else
    {
        // The type of the piece is pawn because it is a promotion
        zobristHash ^= Zobrist::getPieceHash(board.getIndexByPiece(TypePieces::PAWN, piece_depart->getColor()), move.first); // Remove the pawn
        TypePieces promotedType = Piece::charToPieceType(promotionForMove);
        zobristHash ^= Zobrist::getPieceHash(board.getIndexByPiece(promotedType, piece_depart->getColor()), move.second); // add the promote piece
    }

    int iPositionDifferenceAbsolute = std::abs(move.first - move.second);
    int iPositionDifference = move.second - move.first;

    if (piece_depart->getTypePiece() == TypePieces::KING && iPositionDifferenceAbsolute == 2)
    {
        if(move.second%8 == 6) // Roque à droite (petit)
        {
            zobristHash ^= Zobrist::getPieceHash(board.getIndexByPiece(TypePieces::ROOK, piece_depart->getColor()), move.second + 1); // Remove the rook
            zobristHash ^= Zobrist::getPieceHash(board.getIndexByPiece(TypePieces::ROOK, piece_depart->getColor()), move.second - 1); // Add the rook
        }
        else if(move.second%8 == 2) // Roque à droite (petit)
        {
            zobristHash ^= Zobrist::getPieceHash(board.getIndexByPiece(TypePieces::ROOK, piece_depart->getColor()), move.second - 2); // Remove the rook
            zobristHash ^= Zobrist::getPieceHash(board.getIndexByPiece(TypePieces::ROOK, piece_depart->getColor()), move.second + 1); // Add the rook
        }
    }

    // XOR the castling rights
    for (int castlingRight : itabCastlingRights) {
        if(castlingRight != -1)
        {
            zobristHash ^= Zobrist::getHashForCastlingRight(castlingRight);
        }
    }

    // XOR en passant square
    if (enPassantState != -1) {
        zobristHash ^= Zobrist::getHashForEnPassant(enPassantState);
    }
}

//Ne sert plus à rien (à garder pour test dans un premier temps)
int Bot::evaluateMoveWithMinimaxv2(Board& board, int profondeur, bool estMaximisant, int alpha, int beta, const std::pair<int, int>& move, Color currentColor, char& promotion) {
    Piece* capturedPiece = nullptr;
    bool isPromotion = board.isPromotionMove(move.first, move.second, currentColor);
    char promotionForMove = promotion;
    int enPassantPos = -1;
    uint64_t originalHash = board.getZobristHash();
    uint64_t zobristHash = board.getZobristHash();  // Sauvegarder le hash original

    std::string logMessage = "Plateau à la profondeur " + std::to_string(profondeur) + " :\n" + board.getBoardAsString() + "\n";
    m_logFile->logInfo(logMessage + " Move:" + std::to_string(move.first) + "-" + std::to_string(move.second));

    //Exécution du mouvement + récurssif
    board.movePiece(move.first, move.second, currentColor, &capturedPiece, Piece::charToPieceType(promotionForMove), &enPassantPos);

    // Calculer le nouveau hash avant de jouer le coup
    calculateZobristHashForMove(board, move, currentColor, promotionForMove, isPromotion, zobristHash, capturedPiece);
    board.setZobristHash(zobristHash);

    //board.setZobristHash(zobristHash);
    board.setZobristHash(zobristHash);
    int score = alphaBetaWithMemory(board, profondeur - 1, alpha, beta, !estMaximisant, promotionForMove);
    board.undoMove(move.first, move.second, capturedPiece, isPromotion, enPassantPos);

    board.setZobristHash(originalHash);

    return score;
}




//V1 du Bot
void Bot::choisir_meilleur_coup(Board& board, int profondeur_max, std::pair<int, int>& meilleurCoup, char* bestPromotion) {
    int meilleurScore = std::numeric_limits<int>::min();
    m_logFile->clear();
    //clearFile("../debug_log.txt");
    meilleurCoup = { -1, -1 };
    std::pair<int, int> previousBestMove = { -1, -1 };

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
                int score = evaluateMoveWithMinimax(board, profondeur_max, true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), coup, m_color, promoType);
                // Mettre à jour la table de transposition uniquement si le coup est trouvé
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
}


int Bot::alphaBetaBasic(Board& board, int depth, int alpha, int beta, bool estMaximisant, char &bestPromotion) {
    nodeCount++;

    // Cas de base : profondeur 0
    if (depth == 0) {
        int evaluation = board.evaluateTest(m_color);
        return evaluation;
    }

    // Pour les logs
    if (depth == 3) {
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

    //Ehec et mat
    if (possibleMoves.empty()) return estMaximisant ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    int bestScore = estMaximisant ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    char promotion = '\0';

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
                if (score > bestScore) bestScore = score, bestPromotion = promotion; //TODO '\0'
                alpha = std::max(alpha, bestScore);
                if (bestScore >= beta) break;
            } else {
                if (score < bestScore) bestScore = score, bestPromotion = promotion; //TODO '\0'
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
    char promotionForMove = promotion;
    int enPassantPos = -1;
    board.movePiece(move.first, move.second, currentColor, &capturedPiece, Piece::charToPieceType(promotionForMove), &enPassantPos);
    int score = alphaBetaBasic(board, profondeur - 1, alpha, beta, !estMaximisant, promotionForMove);
    board.undoMove(move.first, move.second, capturedPiece, isPromotion);
    return score;
}






