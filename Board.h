//
// Created by Peter on 02/10/2024.
//

#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Type.h"
#include <vector>

class Board {
private:
    Piece* m_tabtabpiBoard[8][8]{};


public:
    Board();
    void initializeBoard();
    Piece* getPieceAt(const Coordinate& in_coord) const;

    Piece* getPieceAt(int in_iRow, int in_iColumn) const;

    bool placePiece(int in_iRow, int in_iCol, Piece* in_pPiece);
    bool movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer);
    bool isMoveValid(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer) const;
    std::vector<Move> listOfPossibleMoves(Color in_colColor) const;
    std::vector<Coordinate> possibleMovesForPiece(const Coordinate& in_coordPiece, const Coordinate* in_optionalCoordTargetPoint = nullptr) const;
    void displayBoard() const;

    std::vector<Coordinate> getMovementsPossibleWithVector(int in_iStartRow, int in_iStartCol, Vector& in_vectMove, const Coordinate* in_coordTargetPoint = nullptr) const;
};

#endif //BOARD_H
