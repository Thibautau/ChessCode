//
// Created by Peter on 29/10/2024.
//

#include "Zobrist.h"
#include <random>

std::vector<std::vector<uint64_t>> zobristTable(64, std::vector<uint64_t>(12));
uint64_t zobristBlackTurn;

void initZobrist() {
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

uint64_t computeZobristHash(const std::vector<int>& board, bool isBlackTurn) {
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
