//
// Created by Peter on 02/10/2024.
//

#include "Piece.h"

Piece::Piece(const TypePieces type, const Color color)
    : m_tpTypePiece(type), m_colColorPiece(color) {
}

TypePieces Piece::getTypePiece() const {
    return m_tpTypePiece;
}

Color Piece::getCouleur() const {
    return m_colColorPiece;
}
