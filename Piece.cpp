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

void Piece::initializePiecePossibleMoves(Piece** board, int in_iPiecePosition)
{
    movePiece(board, in_iPiecePosition);
}

void Piece::movePiece(Piece** board, int newPosition, int oldPosition) {
    int currentRow = newPosition / 8;
    int currentCol = newPosition % 8;
    switch (m_tpTypePiece) {
        case TypePieces::PAWN: {
            int direction = (m_colColorPiece == Color::WHITE) ? 1 : -1;
            int startRow = (m_colColorPiece == Color::WHITE) ? 1 : 6;

            // Normal moves
            int forwardMove = newPosition + direction * 8;
            if (isValidPosition(forwardMove) && board[forwardMove] == nullptr) {
                m_possibleMoves.push_back(forwardMove);
            }

            int twoSteps = newPosition + (direction * 16);
            if (newPosition/8 == startRow) {
                if (isValidPosition(twoSteps) && board[forwardMove] == nullptr && board[twoSteps] == nullptr) {
                    m_possibleMoves.push_back(twoSteps);
                }
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
            addRookMoves(board, newPosition);
            break;
        }
        case TypePieces::KNIGHT: {
            const int *knightMoves = getKnightMoves();
            int numMoves = 8;  // ou la taille appropriée selon tes mouvements de cavalier

            for (int i = 0; i < numMoves; ++i) {
                int knightPosition = newPosition + knightMoves[i];
                if (isValidPosition(knightPosition)) {
                    if (board[knightPosition] == nullptr || board[knightPosition]->getColor() != m_colColorPiece) {
                        m_possibleMoves.push_back(knightPosition);
                    }
                }
            }

        }
        case TypePieces::BISHOP: {
            addBishopMoves(board, newPosition);
            break;
        }
        case TypePieces::QUEEN:
            addRookMoves(board, newPosition); // Mouvements de la tour
            addBishopMoves(board, newPosition);
            break;

        case TypePieces::KING: {
            const int* kingMoves = getKingMoves();
            int numMoves = 8;  // nombre de mouvements possibles pour le roi, à ajuster selon ton cas

            for (int i = 0; i < numMoves; ++i) {
                int kingPosition = newPosition + kingMoves[i];
                if (isValidPosition(kingPosition)) {
                    if (board[kingPosition] == nullptr || board[kingPosition]->getColor() != m_colColorPiece) {
                        m_possibleMoves.push_back(kingPosition);
                    }
                }
            }

        }
        default:
            break;
    }
}

const int* Piece::getKingMoves() {
    static const int kingMoves[8] = {
        -1, 1, -8, 8,
        -7, -9, 7, 9
    };
    return kingMoves;
}

const int* Piece::getKnightMoves() {
    static const int knightMoves[8] = {
        6, 10, 15, 17,
        -6, -10, -15, -17
    };
    return knightMoves;
}



bool Piece::isValidPosition(int position) {
    return position >= 0 && position < 64;
}

void Piece::addRookMoves(Piece** board, int newPosition) {
    int currentCol = newPosition % 8;
    for (int i = 1; i < 8; ++i) {
        // Haut
        int upMove = newPosition + i * 8;
        if (!isValidPosition(upMove)) break;
        if (board[upMove] != nullptr) {
            if (board[upMove]->getColor() != m_colColorPiece) {
                m_possibleMoves.push_back(upMove);
            }
            break;
        }
        m_possibleMoves.push_back(upMove);
    }

    for (int i = 1; i < 8; ++i) {
        // Bas
        int downMove = newPosition - i * 8;
        if (!isValidPosition(downMove)) break;
        if (board[downMove] != nullptr) {
            if (board[downMove]->getColor() != m_colColorPiece) {
                m_possibleMoves.push_back(downMove);
            }
            break;
        }
        m_possibleMoves.push_back(downMove);
    }

    for (int i = 1; i < 8; ++i) {
        // Droite
        int rightMove = newPosition + i;
        if (currentCol+i < 7 && isValidPosition(rightMove)) {
            if (board[rightMove] != nullptr) {
                if (board[rightMove]->getColor() != m_colColorPiece) {
                    m_possibleMoves.push_back(rightMove);
                }
                break;
            }
            m_possibleMoves.push_back(rightMove);
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Gauche
        int leftMove = newPosition - i;
        if (currentCol-i > 0 && isValidPosition(leftMove)) {
            if (board[leftMove] != nullptr) {
                if (board[leftMove]->getColor() != m_colColorPiece) {
                    m_possibleMoves.push_back(leftMove);
                }
                break;
            }
            m_possibleMoves.push_back(leftMove);
        }
    }
}

void Piece::addBishopMoves(Piece** board, int newPosition) {
    int currentCol = newPosition % 8;
    for (int i = 1; i < 8; ++i) {
        // Haut-Droite
        int upRightMove = newPosition + i * 9;
        if (currentCol+i<7 || !isValidPosition(upRightMove)) break;
        if (board[upRightMove] == nullptr) {
            m_possibleMoves.push_back(upRightMove);
        }
        else {
            if (board[upRightMove]->getColor() != m_colColorPiece) {
                m_possibleMoves.push_back(upRightMove);
            }
            break;
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Haut-Gauche
        int upLeftMove = newPosition + i * 7; // 1 ligne en haut, 1 colonne à gauche
        if (currentCol-i>0 || !isValidPosition(upLeftMove)) break;
        if (board[upLeftMove] == nullptr) {
            m_possibleMoves.push_back(upLeftMove);
        }
        else {
            if (board[upLeftMove]->getColor() != m_colColorPiece) {
                m_possibleMoves.push_back(upLeftMove);
            }
            break;
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Bas-Droite
        int downRightMove = newPosition - i * 7; // 1 ligne en bas, 1 colonne à droite
        if (currentCol+i<7 || !isValidPosition(downRightMove)) break;
        if (board[downRightMove] == nullptr) {
            m_possibleMoves.push_back(downRightMove);
        }
        else {
            if (board[downRightMove]->getColor() != m_colColorPiece) {
                m_possibleMoves.push_back(downRightMove);
            }
            break;
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Bas-Gauche
        int downLeftMove = newPosition - i * 9; // 1 ligne en bas, 1 colonne à gauche
        if (currentCol-i>0 || !isValidPosition(downLeftMove)) break;
        if (board[downLeftMove] == nullptr) {
            m_possibleMoves.push_back(downLeftMove);
        }
        else {
            if (board[downLeftMove]->getColor() != m_colColorPiece) {
                m_possibleMoves.push_back(downLeftMove);
            }
            break;
        }
    }
}

