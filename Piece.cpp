//
// Created by Peter on 02/10/2024.
//

#include "Piece.h"

Piece::Piece(const TypePieces type, const Color color)
    : m_tpTypePiece(type), m_colColorPiece(color){m_alreadyMoved = false;}

TypePieces Piece::getTypePiece() const {
    return m_tpTypePiece;
}

Color Piece::getColor() const {
    return m_colColorPiece;
}

bool Piece::hasAlreadyMoved() const
{
    return m_alreadyMoved;
}

void Piece::setAlreadyMoved(bool in_bMoved)
{
    m_alreadyMoved = in_bMoved;
}

Color Piece::getEnemyColor() const
{
    switch (m_colColorPiece)
    {
        case Color::WHITE:
            return Color::BLACK;
        case Color::BLACK:
            return Color::WHITE;
        default:
            return Color::NONE;
    }
}

char Piece::getColorAsChar() const {
    switch (m_colColorPiece) {
        case Color::WHITE:
            return 'W';
        case Color::BLACK:
            return 'B';
        default:
            return '?';
    }
}

char Piece::getDisplayChar() const{
    switch (m_tpTypePiece) {
        case TypePieces::PAWN: return 'p';
        case TypePieces::ROOK: return 'r';
        case TypePieces::KNIGHT: return 'n';
        case TypePieces::BISHOP: return 'b';
        case TypePieces::QUEEN: return 'q';
        case TypePieces::KING: return 'k';
        default: return '?';
    }
}

int Piece::getColumnOfRookAfterRock(int in_iColumn)
{
    if(in_iColumn < 0 || in_iColumn >= 8)
    {
        return -1;
    }


    if(in_iColumn == 0)
    {
        return 3;
    }
    else if(in_iColumn == 7)
    {
        return 5;
    }

    return -1;
}

int Piece::getRookVectorWithAdjustableLength(Vector* out_tabvectRookDisplacement, int in_iIndicesStart, int in_iLengthToAdjust)
{
    if(in_iIndicesStart < 0 || in_iIndicesStart >= 5 || in_iLengthToAdjust > 8 || in_iLengthToAdjust < 0){
        return WRONG_PARAMETER;
    }

    out_tabvectRookDisplacement[in_iIndicesStart] = Vector(1, 0, in_iLengthToAdjust); // To go up
    out_tabvectRookDisplacement[in_iIndicesStart + 1] = Vector(-1, 0, in_iLengthToAdjust); // To go down
    out_tabvectRookDisplacement[in_iIndicesStart + 2] = Vector(0, -1, in_iLengthToAdjust); // To go left
    out_tabvectRookDisplacement[in_iIndicesStart + 3] = Vector(0, 1, in_iLengthToAdjust); // To go right

    return NO_ERROR;
}

int Piece::getBishopVectorWithAdjustableLength(Vector* out_tabvectBishopDisplacement, int in_iIndicesStart, int in_iLengthToAdjust)
{
    if(in_iIndicesStart < 0 || in_iIndicesStart >= 5 || in_iLengthToAdjust > 8 || in_iLengthToAdjust < 0){
        return WRONG_PARAMETER;
    }

    out_tabvectBishopDisplacement[in_iIndicesStart] = Vector(-1, 1, in_iLengthToAdjust); // To go up to the right
    out_tabvectBishopDisplacement[in_iIndicesStart + 1] = Vector(-1, -1, in_iLengthToAdjust); // To go up to the right
    out_tabvectBishopDisplacement[in_iIndicesStart + 2] = Vector(1, 1, in_iLengthToAdjust); // To go down to the right
    out_tabvectBishopDisplacement[in_iIndicesStart + 3] = Vector(1, -1, in_iLengthToAdjust); // To go down to the left

    return NO_ERROR;
}

int Piece::getKnightVectorWithAdjustableLength(Vector* out_tabvectKnightDisplacement, int in_iIndicesStart, int in_iLengthToAdjust)
{
    if(in_iIndicesStart < 0 || in_iLengthToAdjust > 8 || in_iLengthToAdjust < 0){
        return WRONG_PARAMETER;
    }

    out_tabvectKnightDisplacement[in_iIndicesStart] = Vector(-2, -1, in_iLengthToAdjust);
    out_tabvectKnightDisplacement[in_iIndicesStart + 1] = Vector(-1, 1, in_iLengthToAdjust);
    out_tabvectKnightDisplacement[in_iIndicesStart + 2] = Vector(-1, 2, in_iLengthToAdjust);
    out_tabvectKnightDisplacement[in_iIndicesStart + 3] = Vector(1, 2, in_iLengthToAdjust);
    out_tabvectKnightDisplacement[in_iIndicesStart + 4] = Vector(2, 1, in_iLengthToAdjust);
    out_tabvectKnightDisplacement[in_iIndicesStart + 5] = Vector(2, -1, in_iLengthToAdjust);
    out_tabvectKnightDisplacement[in_iIndicesStart + 6] = Vector(1, -2, in_iLengthToAdjust);
    out_tabvectKnightDisplacement[in_iIndicesStart + 7] = Vector(-1, -2, in_iLengthToAdjust);

    return NO_ERROR;
}

TypeOfPieceAttack Piece::typeOfAttack() const
{
    switch (m_tpTypePiece) {
        case TypePieces::PAWN:
            return TypeOfPieceAttack::DIAGONAL;
        case TypePieces::ROOK:
            return TypeOfPieceAttack::STRAIGHT;
        case TypePieces::KNIGHT:
            return TypeOfPieceAttack::L;
        case TypePieces::BISHOP:
            return TypeOfPieceAttack::DIAGONAL;
        case TypePieces::QUEEN:
            return TypeOfPieceAttack::STRAIGHT_AND_DIAGONAL;
        case TypePieces::KING:
            return  TypeOfPieceAttack::STRAIGHT_AND_DIAGONAL;
        default:
            return TypeOfPieceAttack::STRAIGHT;
    }
}

bool Piece::attackStraight() const
{
    TypeOfPieceAttack typeAttack = typeOfAttack();
    return typeAttack == TypeOfPieceAttack::STRAIGHT || typeAttack == TypeOfPieceAttack::STRAIGHT_AND_DIAGONAL;
}

bool Piece::attackDiagonal() const
{
    TypeOfPieceAttack typeAttack = typeOfAttack();
    return typeAttack == TypeOfPieceAttack::DIAGONAL || typeAttack == TypeOfPieceAttack::STRAIGHT_AND_DIAGONAL;
}

bool Piece::attackKnight() const
{
    TypeOfPieceAttack typeAttack = typeOfAttack();
    return typeAttack == TypeOfPieceAttack::L;
}


int Piece::getVectorOfDisplacement(Vector** out_tabvectOfDisplacement, int& out_tabSize)
{
    Vector* vectorOfDisplacement = nullptr;
    int iErrorCode = NO_ERROR;
    int iRow;
    int iLengthToAdjust;
    int iLengthToMoveLeftRight;

    switch (m_tpTypePiece) {
        // The pawn can only move forward
        case TypePieces::PAWN:
            vectorOfDisplacement = new Vector[3];

            // -1 for the black because you want to go down
            iRow = -1;
            if(m_colColorPiece == Color::WHITE)
            {
                // 1 for the white to go up
                iRow = 1;
            }

            iLengthToAdjust = 1;
            if(m_alreadyMoved == false)
            {
                iLengthToAdjust = 2;
            }

            // Moving forward
            vectorOfDisplacement[0] = Vector(iRow, 0, iLengthToAdjust);
            vectorOfDisplacement[1] = Vector(iRow, -1, 1);
            vectorOfDisplacement[2] = Vector(iRow, 1, 1);
            out_tabSize = 3;
            *out_tabvectOfDisplacement = vectorOfDisplacement;
            break;
        case TypePieces::ROOK:
            vectorOfDisplacement = new Vector[4];

            iErrorCode = getRookVectorWithAdjustableLength(vectorOfDisplacement, 0, 7);

            out_tabSize = 4;
            *out_tabvectOfDisplacement = vectorOfDisplacement;
            break;
        case TypePieces::KNIGHT:
            vectorOfDisplacement = new Vector[8];

            iErrorCode = getKnightVectorWithAdjustableLength(vectorOfDisplacement, 0, 1);

            out_tabSize = 8;
            *out_tabvectOfDisplacement = vectorOfDisplacement;
            break;
        case TypePieces::BISHOP:
            vectorOfDisplacement = new Vector[4];

            iErrorCode = getBishopVectorWithAdjustableLength(vectorOfDisplacement, 0, 7);

            out_tabSize = 4;
            *out_tabvectOfDisplacement = vectorOfDisplacement;
            break;
        case TypePieces::QUEEN:
            // BISHOP + ROOK at length 7
            vectorOfDisplacement = new Vector[8];

            iErrorCode = getRookVectorWithAdjustableLength(vectorOfDisplacement, 0, 7);
            iErrorCode = getBishopVectorWithAdjustableLength(vectorOfDisplacement, 4, 7);

            out_tabSize = 8;
            *out_tabvectOfDisplacement = vectorOfDisplacement;
            break;
        case TypePieces::KING:
            // BISHOP + ROOK at length 1
            vectorOfDisplacement = new Vector[8];

            iLengthToMoveLeftRight = 1;
            if(m_alreadyMoved == false)
            {
                iLengthToMoveLeftRight = 2;
            }

            vectorOfDisplacement[0] = Vector(1, 0, 1); // To go up
            vectorOfDisplacement[1] = Vector(-1, 0, 1); // To go down
            vectorOfDisplacement[2] = Vector(0, -1, iLengthToMoveLeftRight); // To go left. Length at 2 for the rock
            vectorOfDisplacement[3] = Vector(0, 1, iLengthToMoveLeftRight); // To go right. Length at 2 for the rock
            iErrorCode = getBishopVectorWithAdjustableLength(vectorOfDisplacement, 4, 1);

            out_tabSize = 8;
            *out_tabvectOfDisplacement = vectorOfDisplacement;
            break;
        default:
            break;
    }

    *out_tabvectOfDisplacement = vectorOfDisplacement;

    return iErrorCode;
}

