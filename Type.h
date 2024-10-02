//
// Created by Peter on 02/10/2024.
//

#ifndef TYPE_H
#define TYPE_H

#include <iostream>

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

    void print() const {
        std::cout << "(" << iRow << ", " << iColumn << ")";
    }
};

struct Move {
    Coordinate coordStart;
    Coordinate coordEnd;
};

struct Vector {
    int iRow;
    int iColumn;
    int iLength;

    Vector() : iRow(0), iColumn(0), iLength(0) {}
    // Constructeur pour initialiser iRow, iColumn et iLength
    Vector(int in_iRow, int in_iColumn, int in_iLength)
        : iRow(in_iRow), iColumn(in_iColumn), iLength(in_iLength) {}
};

#endif //TYPE_H
