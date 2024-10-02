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
    Piece* m_tabtabpiBoard[8][8]{};


public:
    Board();
    void initializeBoard();
    Piece* getPieceAt(const Coordinate& in_coord) const;

    Piece* getPieceAt(int in_iRow, int in_iColumn) const;

    void placePiece(int in_iRow, int in_iCol, Piece* in_pPiece);
    bool movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol);
    bool isMoveValid(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol) const;
    std::vector<Move> listOfPossibleMoves(Color in_colColor) const;
    std::vector<Move> possibleMovesForPiece(const Coordinate& in_coordPiece) const;
};

#endif //BOARD_H
