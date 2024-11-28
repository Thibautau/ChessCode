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
    static uint64_t zobristCastlingRights[4];
    static uint64_t zobristEnPassant[16];
public:
    static void initZobrist();
    static uint64_t getPieceHash(int pieceIndex, int squareIndex);
    static uint64_t computeZobristHash(const std::vector<int>& in_boardVector, bool in_bIsBlackTurn, const std::vector<int>& in_vectCastlingRights, int in_iIndexZobristEnPassant);

};



#endif //ZOBRIST_H
