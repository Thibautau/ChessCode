//
// Created by Peter on 29/10/2024.
//

#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <vector>
#include <cstdint>
#include "Board.h"


class Zobrist {
private:
    static uint64_t zobristTable[781];

public:
    static uint64_t zobristBlackTurn;
    static uint64_t zobristCastlingRights[4];
    static uint64_t zobristEnPassant[8];
    static uint64_t getPieceHash(int pieceIndex, int squareIndex);
    static uint64_t computeZobristHash(const std::vector<int>& in_boardVector, bool in_bIsWhiteTurn, const std::vector<int>& in_vectCastlingRights, int in_iIndexZobristEnPassant, bool in_bEnemyPawnNextToEnPassant = false);
    static uint64_t computeZobristHash(bool in_bIsWhiteTurn, Board& in_board);
    static uint64_t getHashForWhiteTurn();
    static uint64_t getHashForCastlingRight(int in_iIndexForCastling);
    static uint64_t getHashForEnPassant(int in_iIndexForEnPassant);

};



#endif //ZOBRIST_H
