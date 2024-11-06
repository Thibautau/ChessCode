//
// Created by Peter on 29/10/2024.
//

#include "Zobrist.h"
#include <random>


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

    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 12; ++j) {
            zobristTable[i][j] = dis(gen);
        }
    }
    zobristBlackTurn = dis(gen);
}

/**
 * Calcule la valeur de hachage Zobrist pour un état donné de l'échiquier.
 * La fonction prend en compte les pièces sur l'échiquier et le tour en cours (noir ou blanc).
 * Le résultat est une valeur de 64 bits représentant l'état du jeu.
 *
 * @param board Le tableau représentant l'échiquier (64 cases).
 * @param isBlackTurn Booléen indiquant si c'est le tour des noirs.
 * @return La valeur de hachage Zobrist de l'état actuel de l'échiquier.
 */
uint64_t Zobrist::computeZobristHash(const std::vector<int>& board, bool isBlackTurn) {
    uint64_t h = 0;
    if (isBlackTurn) {
        h ^= zobristBlackTurn;
    }

    for (int i = 0; i < 64; ++i) {
        int piece = board[i];
        if (piece != 0) {
            h ^= zobristTable[i][piece - 1];
        }
    }
    return h;
}
