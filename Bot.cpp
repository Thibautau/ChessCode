//
// Created by Peter on 02/10/2024.
//

#include "Bot.h"
#include "Type.h"

#include <cmath>

Bot::Bot(Color color) : m_color(color) {}

Color Bot::getPlayerColor() const {
    return m_color;
}

void Bot::play(Board& board, Coordinate& start, Coordinate& end) {
    Move bestMove;
    int max_depth = 3;
    bestMove = chooseBestMove(board, max_depth, bestMove);

    start = bestMove.coordStart;
    end = bestMove.coordEnd;
}

Move Bot::chooseBestMove(Board& board, int max_depth, Move bestMove) {
    int maxScore = -INFINITY;
    for(Move move: board.listOfPossibleMoves(m_color)) {
        Piece* capturedPiece = nullptr;
        board.movePiece(move.coordStart.iRow, move.coordStart.iColumn, move.coordEnd.iRow, move.coordEnd.iColumn, m_color, &capturedPiece);
        int score = minimax(board, max_depth-1,false);
        board.undoMove(move.coordStart.iRow, move.coordStart.iColumn, move.coordEnd.iRow, move.coordEnd.iColumn, capturedPiece);
        if(score > maxScore) {
            maxScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}

int Bot::minimax(Board& board,int depth,bool isMax) {
    if(depth == 0 || board.isGameOver() == true) {
        return board.evaluate(m_color);
    }
    if(isMax) {
        int maxScore = -INFINITY;
        for(Move move: board.listOfPossibleMoves(m_color)) {
            Piece* capturedPiece = nullptr;
            board.movePiece(move.coordStart.iRow, move.coordStart.iColumn, move.coordEnd.iRow, move.coordEnd.iColumn, m_color, &capturedPiece);
            int score = minimax(board, depth-1, false);
            board.undoMove(move.coordStart.iRow, move.coordStart.iColumn, move.coordEnd.iRow, move.coordEnd.iColumn, capturedPiece);
            maxScore = std::max(maxScore,score);
        }
        return maxScore;
    }
    else {
        int maxScore = INFINITY;
        for(Move move: board.listOfPossibleMoves(m_color)) {
            Piece* capturedPiece = nullptr;
            board.movePiece(move.coordStart.iRow, move.coordStart.iColumn, move.coordEnd.iRow, move.coordEnd.iColumn, m_color, &capturedPiece);
            int score = minimax(board, depth-1, true);
            board.undoMove(move.coordStart.iRow, move.coordStart.iColumn, move.coordEnd.iRow, move.coordEnd.iColumn, capturedPiece);
            maxScore = std::min(maxScore, score);
        }
        return maxScore;
    }
}