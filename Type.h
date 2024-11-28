//
// Created by Peter on 02/10/2024.
//

#ifndef TYPE_H
#define TYPE_H

#include <iostream>

enum class TypePieces { PAWN, KING, QUEEN, ROOK, BISHOP, KNIGHT, NONE };
enum class Color { WHITE, BLACK, NONE };
enum class PlayerType {HUMAN,BOT};
enum class GameMode {JVJ, JVB, BVJ, BVB};
enum class TypeOfPieceAttack {STRAIGHT, DIAGONAL, L, STRAIGHT_AND_DIAGONAL};

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

    // Surcharge de l'opérateur == pour comparer deux objets Coordinate
    bool operator==(const Coordinate& in_other) const {
        return iRow == in_other.iRow && iColumn == in_other.iColumn;
    }
};

#endif //TYPE_H
