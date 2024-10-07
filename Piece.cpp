//
// Created by Peter on 02/10/2024.
//

#include "Piece.h"

Piece::Piece(TypePieces type, Color color)
    : m_tpTypePiece(type), m_colColorPiece(color) {
}

TypePieces Piece::getTypePiece() const {
    return m_tpTypePiece;
}

Color Piece::getColor() const {
    return m_colColorPiece;
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

const std::vector<int>& Piece::getPossibleMoves() const {
    return m_possibleMoves;
}

void Piece::movePiece(Piece** board, int newPosition, int oldPosition) {
    int currentRow = newPosition / 8;
    int currentCol = newPosition % 8;
    switch (m_tpTypePiece) {
        case TypePieces::PAWN: {
            int direction = (m_colColorPiece == Color::WHITE) ? 1 : -1;

            // Normal moves
            int forwardMove = newPosition + direction * 8;
            if (isValidPosition(forwardMove) && board[forwardMove] == nullptr) {
                m_possibleMoves.push_back(forwardMove);
            }

            // Capture moves
            int captureLeft = newPosition + direction * 8 - 1;
            int captureRight = newPosition + direction * 8 + 1;
            if (isValidPosition(captureLeft) && board[captureLeft] != nullptr && board[captureLeft]->getColor() != m_colColorPiece) {
                m_possibleMoves.push_back(captureLeft);
            }
            if (isValidPosition(captureRight) && board[captureRight] != nullptr && board[captureRight]->getColor() != m_colColorPiece) {
                m_possibleMoves.push_back(captureRight);
            }
            break;
        }
        case TypePieces::ROOK: {
            for (int i = 1; i < 8; ++i) {
                // Haut
                int upMove = newPosition + i * 8;
                if (!isValidPosition(upMove)) break;
                m_possibleMoves.push_back(upMove);
                if (board[upMove] != nullptr) {
                    if (board[upMove]->getColor() != m_colColorPiece) {
                        m_possibleMoves.push_back(upMove);
                    }
                    break;
                }

                // Bas
                int downMove = newPosition - i * 8;
                if (!isValidPosition(downMove)) break;
                m_possibleMoves.push_back(downMove);
                if (board[downMove] != nullptr) {
                    if (board[downMove]->getColor() != m_colColorPiece) {
                        m_possibleMoves.push_back(downMove);
                    }
                    break;
                }

                // Droite
                int rightMove = newPosition + i;
                if (currentCol < 7 && isValidPosition(rightMove)) {
                    m_possibleMoves.push_back(rightMove);
                    if (board[rightMove] != nullptr) {
                        if (board[rightMove]->getColor() != m_colColorPiece) {
                            m_possibleMoves.push_back(rightMove);
                        }
                        break;
                    }
                }

                // Gauche
                int leftMove = newPosition - i;
                if (currentCol > 0 && isValidPosition(leftMove)) {
                    m_possibleMoves.push_back(leftMove);
                    if (board[leftMove] != nullptr) {
                        if (board[leftMove]->getColor() != m_colColorPiece) {
                            m_possibleMoves.push_back(leftMove);
                        }
                        break;
                    }
                }
            }
            break;
        }
        case TypePieces::KNIGHT:
            // Add moves for the knight
                break;
        case TypePieces::BISHOP:
            // Add moves for the bishop
                break;
        case TypePieces::QUEEN:
            // Add moves for the queen
                break;
        case TypePieces::KING:
            // Add moves for the king
                break;
        default:
            break;
    }
}

bool Piece::isValidPosition(int position) {
    return position >= 0 && position < 64;
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

