//
// Created by Peter on 02/10/2024.
//

#ifndef TYPE_H
#define TYPE_H

enum class TypePieces { PAWN, KING, QUEEN, ROOK, BISHOP, KNIGHT };
enum class Color { WHITE, BLACK, NONE };

struct Coordinate {
    int iRow;
    int iColumn;
    Coordinate() : iRow(-1), iColumn(-1) {}

    // Constructeur pour initialiser iRow et iColumn
    Coordinate(int in_iRow, int in_iColumn) : iRow(in_iRow), iColumn(in_iColumn) {}

    int getRow() const {
        return iRow;
    }

    // Accesseur pour iColonne
    int getColumn() const {
        return iColumn;
    }
};

struct Move {
    Coordinate coordStart;
    Coordinate coordEnd;
};

#endif //TYPE_H
