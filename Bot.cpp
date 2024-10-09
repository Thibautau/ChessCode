//
// Created by Peter on 02/10/2024.
//

#include "Bot.h"
#include "Type.h"

#include <cmath>
#include <cstdlib>

Bot::Bot(Color color) : m_color(color) {}

Color Bot::getPlayerColor() const {
    return m_color;
}

void Bot::play(Board& board, Coordinate& start, Coordinate& end) {
    std::vector<Move> possibleMoves = board.listOfPossibleMoves(m_color);

    if (possibleMoves.empty()) {
        return;
    }

    int randomIndex = rand() % possibleMoves.size();

    start = possibleMoves[randomIndex].coordStart;
    end = possibleMoves[randomIndex].coordEnd;
}

Move Bot::chooseBestMove(Board& board, int max_depth) {
    int maxScore = -INFINITY;
    Move bestMove = {{1,0},{2,0}};
    for(Move move: board.listOfPossibleMoves(m_color)) {
        board.movePiece(move.coordStart.iRow, move.coordStart.iColumn, move.coordEnd.iRow, move.coordEnd.iColumn, m_color);
        int score = minimax(board, max_depth-1,false);
        if(score > maxScore) {
            maxScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}

int Bot::minimax(Board& board,int depth,bool isMax) {
    if(depth == 0) {
        return board.evaluate(m_color);
    }
    if(isMax) {
        int maxScore = -INFINITY;
        for(Move move: board.listOfPossibleMoves(m_color)) {
            board.movePiece(move.coordStart.iRow, move.coordStart.iColumn, move.coordEnd.iRow, move.coordEnd.iColumn, m_color);
            int score = minimax(board, depth-1, false);
            maxScore = std::max(maxScore,score);
        }
        return maxScore;
    }
    else {
        int maxScore = INFINTY;
        for(Move move: board.listOfPossibleMoves(m_color)) {
            board.movePiece(move.coordStart.iRow, move.coordStart.iColumn, move.coordEnd.iRow, move.coordEnd.iColumn, m_color);
            int score = minimax(board, depth-1, true);
            maxScore = std::min(maxScore, score);
        }
        return maxScore;
    }
}