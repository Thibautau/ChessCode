//
// Created by Peter on 02/10/2024.
//

#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Type.h"
#include <vector>
#include <memory>

class Board {
private:
    std::vector<std::vector<std::unique_ptr<Piece>>> m_tabTabPiBoard;

public:
    Board();
    void initializeBoard();
    Piece* getPieceAt(const Coordinate& coord) const;
    void placePiece(int row, int col, std::unique_ptr<Piece> piece);
    bool movePiece(int startRow, int startCol, int endRow, int endCol);
    bool isMoveValid(int startRow, int startCol, int endRow, int endCol) const;
    std::vector<Move> listOfPossibleMoves(Color in_colColor) const;
    std::vector<Move> possibleMovesForPiece(const Coordinate& coordPiece) const;
};

#endif //BOARD_H
