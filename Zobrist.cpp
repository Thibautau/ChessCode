//
// Created by Peter on 29/10/2024.
//

#include "Zobrist.h"
#include <random>
#include <stdexcept>


/**
 * Table de hachage Zobrist.
 * Chaque case du tableau représente un emplacement sur l'échiquier (64 cases)
 * et chaque pièce a 12 valeurs possibles (une pour chaque type de pièce, y compris les couleurs).
 */
std::vector<std::vector<uint64_t>> Zobrist::zobristTable(64, std::vector<uint64_t>(12));

/**
 * Variable qui stocke la valeur de hachage pour le tour des noirs.
 */
uint64_t Zobrist::zobristBlackTurn;

//NEW
// Table pour les droits de roque (4 valeurs possibles)
uint64_t Zobrist::zobristCastlingRights[4];

// Table pour la prise en passant (16 cases)
uint64_t Zobrist::zobristEnPassant[16];

/**
 * Initialise la table Zobrist avec des valeurs aléatoires.
 * La fonction génère des nombres aléatoires et les assigne à la table de hachage.
 * Un générateur de nombres aléatoires est utilisé
 * pour garantir l'unicité des valeurs de hachage.
 */
void Zobrist::initZobrist() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;

    // Génération des clés pour chaque combinaison de case et type de pièce
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 12; ++j) {
            zobristTable[i][j] = dis(gen);
        }
    }
    zobristBlackTurn = dis(gen);

    //NEW
    // Génération des clés pour les droits de roque
    for (int i = 0; i < 4; ++i) {
        zobristCastlingRights[i] = dis(gen);
    }

    // Génération des clés pour la prise en passant
    for (int i = 0; i < 16; ++i) {
        zobristEnPassant[i] = dis(gen);
    }
    //ENDNEW
}

/**
 * Calcule la valeur de hachage Zobrist pour un état donné de l'échiquier.
 * La fonction prend en compte les pièces sur l'échiquier et le tour en cours (noir ou blanc).
 * Le résultat est une valeur de 64 bits représentant l'état du jeu.
 *
 * @param in_boardVectorIndexInZobristTable Tableau de 64 contenant les indices de chaque case dans la table de Zobrits ex : case 0 (tour blanche) => indice ( 2 (tour) + 0 (6 si noir))
 * @param in_bIsBlackTurn Booléen indiquant si c'est le tour des noirs.
 * @param in_vectIndexCastlingRights Les droits de roque sous forme d'indice (0-3). Même principe que in_boardVectorIndexInZobristTable
 * @param in_iIndexZobristEnPassant La case de prise en passant (de 0 à 15) ou -1 si pas de prise en passant.
 * @return La valeur de hachage Zobrist de l'état actuel de l'échiquier.
 */
uint64_t Zobrist::computeZobristHash(const std::vector<int>& in_boardVectorIndexInZobristTable, bool in_bIsBlackTurn, const std::vector<int>& in_vectIndexCastlingRights, int in_iIndexZobristEnPassant) {
    uint64_t hashedBoard = 0;
    if (in_bIsBlackTurn) {
        hashedBoard ^= zobristBlackTurn;
    }

    for (int i = 0; i < 64; ++i) {
        int iIndexZobristPiece = in_boardVectorIndexInZobristTable[i];
        if (iIndexZobristPiece != 0) {
            hashedBoard ^= zobristTable[i][iIndexZobristPiece - 1];
        }
    }

    // Ajouter la clé pour les droits de roque
    for (int i = 0; i < 4; ++i) {
        int iIndiceCastling = in_vectIndexCastlingRights[i];
        if (iIndiceCastling != -1) // Si on a le droit de roque
        {
            hashedBoard ^= zobristCastlingRights[iIndiceCastling];
        }
    }

    // Ajouter la clé pour la prise en passant si applicable
    if (in_iIndexZobristEnPassant != -1) {
        hashedBoard ^= zobristEnPassant[in_iIndexZobristEnPassant];
    }

    return hashedBoard;
}

uint64_t Zobrist::getPieceHash(int pieceIndex, int squareIndex) {
    if (squareIndex < 0 || squareIndex >= 64 || pieceIndex <= 0 || pieceIndex > 12) {
        throw std::out_of_range("Invalid square or piece index for Zobrist hash.");
    }
    return zobristTable[squareIndex][pieceIndex];
}
