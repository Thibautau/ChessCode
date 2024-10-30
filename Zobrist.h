//
// Created by Peter on 29/10/2024.
//

#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <vector>
#include <cstdint>


class Zobrist {
private:
    const int PION_BLANC  = 1;
    const int TOUR_BLANC = 2;
    const int CAVALIER_BLANC = 3;
    const int FOU_BLANC = 4;
    const int DAME_BLANC = 5;
    const int ROI_BLANC = 6;

    const int PION_NOIR = 7;
    const int TOUR_NOIR = 8;
    const int CAVALIER_NOIR = 9;
    const int FOU_NOIR = 10;
    const int DAME_NOIR = 11;
    const int ROI_NOIR = 12;
    static std::vector<std::vector<uint64_t>> zobristTable;
    static uint64_t zobristBlackTurn;
public:
    static void initZobrist();
    static uint64_t computeZobristHash(const std::vector<int>& board, bool isBlackTurn);

};



#endif //ZOBRIST_H
