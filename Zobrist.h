//
// Created by Peter on 29/10/2024.
//

#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <vector>
#include <cstdint>


class Zobrist {
private:
    static std::vector<std::vector<uint64_t>> zobristTable;
    static uint64_t zobristBlackTurn;
public:
    static void initZobrist();
    static uint64_t computeZobristHash(const std::vector<int>& board, bool isBlackTurn);

};



#endif //ZOBRIST_H
