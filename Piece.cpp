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

std::vector<int> Piece::getPossibleMoves(Piece **board, int in_iPiecePosition) const {
    return movePiece(board, in_iPiecePosition);
}

std::vector<int> Piece::movePiece(Piece** board, int iposition) const {
    std::vector<int> possibleMoves;
    int iUseless1, iUseless2;
    switch (m_tpTypePiece) {
        case TypePieces::PAWN: {
            int direction = (m_colColorPiece == Color::WHITE) ? 1 : -1;
            int startRow = (m_colColorPiece == Color::WHITE) ? 1 : 6;

            // Normal moves
            int forwardMove = iposition + direction * 8;
            if (isValidPosition(forwardMove) && board[forwardMove] == nullptr) {
                possibleMoves.push_back(forwardMove);
            }

            int twoSteps = iposition + (direction * 16);
            if (iposition/8 == startRow) {
                if (isValidPosition(twoSteps) && board[forwardMove] == nullptr && board[twoSteps] == nullptr) {
                    possibleMoves.push_back(twoSteps);
                }
            }

            // Capture moves
            int captureLeft = iposition + direction * 8 - 1;
            int captureRight = iposition + direction * 8 + 1;
            if (isValidPosition(captureLeft) && board[captureLeft] != nullptr && board[captureLeft]->getColor() != m_colColorPiece) {
                possibleMoves.push_back(captureLeft);
            }
            if (isValidPosition(captureRight) && board[captureRight] != nullptr && board[captureRight]->getColor() != m_colColorPiece) {
                possibleMoves.push_back(captureRight);
            }
            break;
        }
        case TypePieces::ROOK: {
            addRookMoves(board, iposition, possibleMoves);
            break;
        }
        case TypePieces::KNIGHT: {
            const int *knightMoves = getKnightMoves(iUseless1, iUseless2);
            int numMoves = 8;

            for (int i = 0; i < numMoves; ++i) {
                int knightPosition = iposition + knightMoves[i];
                if (isValidPosition(knightPosition)) {
                    if (board[knightPosition] == nullptr || board[knightPosition]->getColor() != m_colColorPiece) {
                        possibleMoves.push_back(knightPosition);
                    }
                }
            }

        }
        case TypePieces::BISHOP: {
            addBishopMoves(board, iposition, possibleMoves);
            break;
        }
        case TypePieces::QUEEN:
            addRookMoves(board, iposition, possibleMoves); // Mouvements de la tour
            addBishopMoves(board, iposition, possibleMoves);
            break;

        case TypePieces::KING: {
            const int* kingMoves = getKingMoves(iUseless1, iUseless2);
            int numMoves = 8;

            for (int i = 0; i < numMoves; ++i) {
                int kingPosition = iposition + kingMoves[i];
                if (isValidPosition(kingPosition)) {
                    if (board[kingPosition] == nullptr || board[kingPosition]->getColor() != m_colColorPiece) {
                        possibleMoves.push_back(kingPosition);
                    }
                }
            }

        }
        default:
            break;
    }
    return possibleMoves;
}

int* Piece::getKingMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement)
{
    out_iNbOfRepetitionToDo = 1;
    out_iNbOfMovement = 8;
    static int kingMoves[8] = {
        -1, 1,-8, 8,
        -7, -9, 7, 9
    };
    return kingMoves;
}

int* Piece::getRockMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement) {
    out_iNbOfRepetitionToDo = 1;
    out_iNbOfMovement = 2;
    static int kingMoves[2] = {
        2, -2
    };
    return kingMoves;
}

int* Piece::getKnightMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement) {
    out_iNbOfRepetitionToDo = 1;
    out_iNbOfMovement = 8;
    static int knightMoves[8] = {
        6, 10, 15, 17,
        -6, -10, -15, -17
    };
    return knightMoves;
}

int* Piece::getRookMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement) {
    out_iNbOfRepetitionToDo = 7;
    out_iNbOfMovement = 4;
    static int rookMoves[4] = {
        8, // Haut
        -8, // Bas
        -1, // Gauche
        1 // Droit
    };
    return rookMoves;
}

int* Piece::getBishopMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement) {
    out_iNbOfRepetitionToDo = 7;
    out_iNbOfMovement = 4;
    static int bishopMoves[4] = {
        7, // Haut gauche
        9, // Haut droite
        -7, //Bas droite (c'est bien ça!)
        -9 // Bas gauche
    };
    return bishopMoves;
}

int* Piece::getQueenMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement) {
    out_iNbOfRepetitionToDo = 7;
    out_iNbOfMovement = 8;
    static int queenMoves[8] = {
        8, // Haut
        -8, // Bas
        -1, // Gauche
        1, // Droit
        7, // Haut gauche
        9, // Haut droite
        -7, //Bas droite (c'est bien ça!)
        -9 // Bas gauche
    };
    return queenMoves;
}

int* Piece::getPawnMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement, Color in_colPiece) {
    out_iNbOfRepetitionToDo = 2;
    if(in_colPiece == Color::WHITE)
    {
        static int queenMoves[4] = {
            8, 7, 9, 16
        };
        return queenMoves;
    }
    else if(in_colPiece == Color::BLACK)
    {
        static int queenMoves[4] = {
            -8, -7, -9, -16
        };
        return queenMoves;
    }
    return {};
}


bool Piece::isValidPosition(int position) {
    return position >= 0 && position < 64;
}


bool Piece::isBishopNextPositionValid(int in_iDirection, int in_iNextPosition)
{
    if(in_iNextPosition < 0 || in_iNextPosition >= 64)
    {
        return false;
    }

    switch (in_iDirection) {
        case 9:
        case -7:
            return in_iNextPosition % 8 != 0;  // Si on dépasse la bordure droite

        case 7:
        case -9:
            return in_iNextPosition % 8 != 7;  // Si on dépasse la bordure gauche

        default:
            return false;  // Pour toute autre direction non gérée
    }
}


bool Piece::isRookNextPositionValid(int in_iDirection, int in_iNextPosition)
{
    if(in_iNextPosition < 0 || in_iNextPosition >= 64) // Verify is the rook can go up or down
    {
        return false;
    }

    switch (in_iDirection) {
        case -8:
        case 8:
            return isValidPosition(in_iNextPosition);
        case 1:
            return in_iNextPosition % 8 < 8;  // Si on dépasse la bordure droite
        case -1:
            return in_iNextPosition >= 0; // Si on dépasse la bordure gauche

        default:
            return false;  // Pour toute autre direction non gérée
    }
}

bool Piece::isKnightNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition)
{
    if(in_iNextPosition < 0 || in_iNextPosition >= 64) // Verify is the rook can go up or down
    {
        return false;
    }

    int iInitialRow = in_iInitialPosition / 8;
    int iInitialColumn = in_iInitialPosition % 8;

    int iNextRow = in_iNextPosition / 8;
    int iNextColumn = in_iNextPosition % 8;

    int iRowDifference = std::abs(iInitialRow - iNextRow);
    int iColumnDifference = std::abs(iInitialColumn - iNextColumn);

    if(iRowDifference == 1 && iColumnDifference == 2)
    {
        return true;
    }
    if(iRowDifference == 2 && iColumnDifference == 1)
    {
        return true;
    }
    return false;
}

bool Piece::isPawnNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition)
{
    if(in_iNextPosition < 0 || in_iNextPosition >= 64) // Verify is the rook can go up or down
    {
        return false;
    }

    int direction = (m_colColorPiece == Color::WHITE) ? 1 : -1;
    int startRow = (m_colColorPiece == Color::WHITE) ? 1 : 6;

    int iInitialRow = in_iInitialPosition / 8;

    bool bCanMoveOf2 = false;
    if(iInitialRow == startRow)
    {
        bCanMoveOf2 = true;
    }
    switch (in_iDirection) {
        case 16:
            return isValidPosition(in_iNextPosition) && bCanMoveOf2 && m_colColorPiece == Color::WHITE;
        case -16:
            return isValidPosition(in_iNextPosition) && bCanMoveOf2 && m_colColorPiece == Color::BLACK;
        case -8:
            return isValidPosition(in_iNextPosition) && m_colColorPiece == Color::BLACK;
        case 8:
            return isValidPosition(in_iNextPosition) && m_colColorPiece == Color::WHITE;
        case 9:
        case -7:
            return in_iNextPosition % 8 != 0;  // Si on dépasse la bordure droite

        case 7:
        case -9:
            return in_iNextPosition % 8 != 7;  // Si on dépasse la bordure gauche

        default:
            return false;  // Pour toute autre direction non gérée
    }
}

bool Piece::isNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition)
{
    if(in_iNextPosition < 0 || in_iNextPosition >= 64)
    {
        return false;
    }

    switch (m_tpTypePiece) {
        case TypePieces::BISHOP:
            return isBishopNextPositionValid(in_iDirection, in_iNextPosition);
        case TypePieces::ROOK:
            return isRookNextPositionValid(in_iDirection, in_iNextPosition);
        case TypePieces::QUEEN:
            return isRookNextPositionValid(in_iDirection, in_iNextPosition) || isBishopNextPositionValid(in_iDirection, in_iNextPosition);
        case TypePieces::KING:
            return isRookNextPositionValid(in_iDirection, in_iNextPosition) || isBishopNextPositionValid(in_iDirection, in_iNextPosition);
        case TypePieces::KNIGHT:
            return isKnightNextPositionValid(in_iDirection, in_iInitialPosition, in_iNextPosition);
        case TypePieces::PAWN:
            return isPawnNextPositionValid(in_iDirection, in_iInitialPosition, in_iNextPosition);
    }

    return false;
}

void Piece::addBishopMoves(Piece** board, int newPosition, std::vector<int>& possibleMoves) {
    int currentCol = newPosition % 8;
/*
    // Haut-Droite
    for (int i = 1; i < 8; ++i) {
        int upRightMove = newPosition + i * 9; // 1 ligne en haut, 1 colonne à droite
        if (upRightMove % 8 == 0) break; // Si on dépasse la bordure droite
        if (!isValidPosition(upRightMove)) break;
        if (board[upRightMove] == nullptr) {
            possibleMoves.push_back(upRightMove);
        } else {
            if (board[upRightMove]->getColor() != m_colColorPiece) {
                possibleMoves.push_back(upRightMove);
            }
            break;
        }
    }

    // Haut-Gauche
    for (int i = 1; i < 8; ++i) {
        int upLeftMove = newPosition + i * 7; // 1 ligne en haut, 1 colonne à gauche
        if (upLeftMove % 8 == 7) break; // Si on dépasse la bordure gauche
        if (!isValidPosition(upLeftMove)) break;
        if (board[upLeftMove] == nullptr) {
            possibleMoves.push_back(upLeftMove);
        } else {
            if (board[upLeftMove]->getColor() != m_colColorPiece) {
                possibleMoves.push_back(upLeftMove);
            }
            break;
        }
    }

    // Bas-Droite
    for (int i = 1; i < 8; ++i) {
        int downRightMove = newPosition - i * 7; // 1 ligne en bas, 1 colonne à droite
        if (downRightMove % 8 == 0) break; // Si on dépasse la bordure droite
        if (!isValidPosition(downRightMove)) break;
        if (board[downRightMove] == nullptr) {
            possibleMoves.push_back(downRightMove);
        } else {
            if (board[downRightMove]->getColor() != m_colColorPiece) {
                possibleMoves.push_back(downRightMove);
            }
            break;
        }
    }

    // Bas-Gauche
    for (int i = 1; i < 8; ++i) {
        int downLeftMove = newPosition - i * 9; // 1 ligne en bas, 1 colonne à gauche
        if (downLeftMove % 8 == 7) break; // Si on dépasse la bordure gauche
        if (!isValidPosition(downLeftMove)) break;
        if (board[downLeftMove] == nullptr) {
            possibleMoves.push_back(downLeftMove);
        } else {
            if (board[downLeftMove]->getColor() != m_colColorPiece) {
                possibleMoves.push_back(downLeftMove);
            }
            break;
        }
    }*/
}


void Piece::addRookMoves(Piece** board, int newPosition, std::vector<int>& possibleMoves) {
    int currentCol = newPosition % 8;
    /*for (int i = 1; i < 8; ++i) {
        // Haut
        int upMove = newPosition + i * 8;
        if (!isValidPosition(upMove)) break;
        if (board[upMove] != nullptr) {
            if (board[upMove]->getColor() != m_colColorPiece) {
                possibleMoves.push_back(upMove);
            }
            break;
        }
        possibleMoves.push_back(upMove);
    }

    for (int i = 1; i < 8; ++i) {
        // Bas
        int downMove = newPosition - i * 8;
        if (!isValidPosition(downMove)) break;
        if (board[downMove] != nullptr) {
            if (board[downMove]->getColor() != m_colColorPiece) {
                possibleMoves.push_back(downMove);
            }
            break;
        }
        possibleMoves.push_back(downMove);
    }

    for (int i = 1; i < 8; ++i) {
        // Droite
        int rightMove = newPosition + i;
        if (currentCol+i < 7 && isValidPosition(rightMove)) {
            if (board[rightMove] != nullptr) {
                if (board[rightMove]->getColor() != m_colColorPiece) {
                    possibleMoves.push_back(rightMove);
                }
                break;
            }
            possibleMoves.push_back(rightMove);
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Gauche
        int leftMove = newPosition - i;
        if (currentCol-i > 0 && isValidPosition(leftMove)) {
            if (board[leftMove] != nullptr) {
                if (board[leftMove]->getColor() != m_colColorPiece) {
                    possibleMoves.push_back(leftMove);
                }
                break;
            }
            possibleMoves.push_back(leftMove);
        }
    }*/
}