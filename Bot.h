//
// Created by Peter on 02/10/2024.
//

#ifndef BOT_H
#define BOT_H

#include "Type.h"
#include "Board.h"
#include "Player.h"
#include <unordered_map>
#include <cstdint>

const int EXACT = 0;
const int ALPHA_CUT = -1;
const int BETA_CUT = 1;

struct TranspositionTableEntry {
    int depth;
    int score;
    int flag; // 0: Exact, -1: Alpha cut, 1: Beta cut
};

class Bot : public Player {
private:
    Color m_color;
    std::unordered_map<uint64_t, TranspositionTableEntry> transpositionTable;


public:
    static int nodeCount;
    Bot(Color color);
    void play(Board& board, int& start, int& end) override;
    void playWithDepth(Board &board, int &start, int &end, int depth, char &promotion);
    Color getPlayerColor() const override;
    void choisir_meilleur_coup(Board &board, int profondeur_max, std::pair<int, int> &meilleurCoup, char *bestPromotion=nullptr);
    int minimax(Board &board, int profondeur, bool estMaximisant, int alpha, int beta, char &bestPromotion);
    int evaluateMoveWithMinimax(Board& board, int profondeur, bool estMaximisant, int alpha, int beta, const std::pair<int, int>& move, Color currentColor,char& promotion);
};

#endif //BOT_H
