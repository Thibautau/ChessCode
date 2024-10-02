//
// Created by Peter on 02/10/2024.
//

#ifndef PIECE_H
#define PIECE_H

#include "Type.h"

class Piece {
private:
    TypePieces m_tpTypePiece;
    Color m_colColorPiece;
    Coordinate m_coordPiece;

public:
    Piece(TypePieces type, Color color);
    TypePieces getTypePiece() const;
    Color getCouleur() const;
};

#endif //PIECE_H
