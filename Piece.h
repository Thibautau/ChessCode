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

public:
    Piece(TypePieces type, Color color);
    TypePieces getTypePiece() const;
    void setTypePiece(TypePieces type);
    Color getColor() const;
    Color getEnemyColor() const;
    char getColorAsChar() const;
    char getDisplayChar() const;
    static int getColumnOfRookAfterRock(int in_iColumn);
    TypeOfPieceAttack typeOfAttack() const;
    bool attackStraight() const;
    bool attackDiagonal() const;
    bool attackKnight() const;

    static int* getKingMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement) ;
    static int* getKnightMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    static int* getRookMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    static int* getRockMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    static int* getBishopMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    static int* getQueenMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    static int* getPawnMoves(int &out_iNbOfRepetitionToDo, int& out_iNbOfMovement, Color in_colPiece);


    static bool isValidPosition(int position);

    bool isBishopNextPositionValid(int in_iDirection, int in_iNextPosition);
    bool isRookNextPositionValid(int in_iDirection, int in_iNextPosition);
    bool isKnightNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition);
    bool isPawnNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition);
    bool isNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition);
};

#endif //PIECE_H
