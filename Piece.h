//
// Created by Peter on 02/10/2024.
//

#ifndef PIECE_H
#define PIECE_H

#define NO_ERROR            0x0000
#define WRONG_PARAMETER     0x0001
#define ERROR               0x0002

#include "Type.h"
#include <vector>

class Piece {
private:
    TypePieces m_tpTypePiece;
    Color m_colColorPiece;
    std::vector<int> m_possibleMoves;

public:
    Piece(TypePieces type, Color color);
    TypePieces getTypePiece() const;
    Color getColor() const;
    Color getEnemyColor() const;
    char getColorAsChar() const;
    char getDisplayChar() const;
    static int getColumnOfRookAfterRock(int in_iColumn);
    TypeOfPieceAttack typeOfAttack() const;
    bool attackStraight() const;
    bool attackDiagonal() const;
    bool attackKnight() const;
    const std::vector<int>& getPossibleMoves() const;

    void movePiece(Piece**, int newPosition, int oldPosition = 0);
    static const int* getKingMoves() ;
    static const int* getKnightMoves() ;

    static bool isValidPosition(int position);

    void addRookMoves(Piece** board, int newPosition);
    void addBishopMoves(Piece** board, int newPosition);
};

#endif //PIECE_H
