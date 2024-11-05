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

void Piece::setTypePiece(TypePieces type) {
    m_tpTypePiece = type;
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

Color Piece::getEnemyColor(Color in_colPiece)
{
    switch (in_colPiece)
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

TypePieces Piece::charToPieceType(char piece) {
    switch (piece) {
        case 'p': return TypePieces::PAWN;
        case 'n': return TypePieces::KNIGHT;
        case 'b': return TypePieces::BISHOP;
        case 'r': return TypePieces::ROOK;
        case 'q': return TypePieces::QUEEN;
        case 'k': return TypePieces::KING;
        default: return TypePieces::NONE;
    }
}

std::vector<TypePieces> Piece::pieceForPromotion() {
    std::vector<TypePieces> promotionOptions = {
        TypePieces::ROOK,
        TypePieces::KNIGHT,
        TypePieces::BISHOP,
        TypePieces::QUEEN
    };
    return promotionOptions;
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

int* Piece::getLittleRockMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement)
{
    out_iNbOfRepetitionToDo = 1;
    out_iNbOfMovement = 1;

    static int kingMoves[1] = {
        2
    };
    return kingMoves;
}
int* Piece::getBigRockMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement)
{
    out_iNbOfRepetitionToDo = 1;
    out_iNbOfMovement = 1;

    static int kingMoves[1] = {
        -2
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
            return in_iNextPosition % 8 != 0;  // Si on dépasse la bordure droite
        case -1:
            return in_iNextPosition % 8 != 7;  // Si on dépasse la bordure gauche

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

bool Piece::isRockNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition) const {
    if(in_iNextPosition < 0 || in_iNextPosition >= 64) // Verify is the rook can go up or down
    {
        return false;
    }

    int iPositionDifference = std::abs(in_iInitialPosition - in_iNextPosition);
    if(iPositionDifference != 2)
    {
        return false;
    }

    switch (in_iDirection) {
        case -2:
            switch(m_colColorPiece)
            {
                case Color::WHITE:
                    return in_iNextPosition == 2;
                case Color::BLACK:
                    return in_iNextPosition == 58;
                default:
                    return false;
            }
        case 2:
            switch(m_colColorPiece)
            {
                case Color::WHITE:
                    return in_iNextPosition == 6;
                case Color::BLACK:
                    return in_iNextPosition == 62;
                default:
                    return false;
            }

        default:
            return false;
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