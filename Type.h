//
// Created by Peter on 02/10/2024.
//

#ifndef TYPE_H
#define TYPE_H

enum class TypePieces { PAWN, KING, QUEEN, ROOK, BISHOP, KNIGHT };
enum class Color { WHITE, BLACK };

struct Coordinate {
    int iLigne;
    int iColonne;

    Coordinate(int in_iLigne = 0, int in_iColonne = 0)
        : iLigne(in_iLigne), iColonne(in_iColonne) {}


    int getLigne() const {
        return iLigne;
    }

    // Accesseur pour iColonne
    int getColonne() const {
        return iColonne;
    }
};

struct Move {
    Coordinate start;
    Coordinate end;
};

#endif //TYPE_H
