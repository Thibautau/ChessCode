//
// Created by Peter on 02/10/2024.
//

#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Type.h"
#include <vector>

class Board {
private:
    Piece* m_tabtabpiBoard[8][8]{};
    Coordinate m_enPassantPosition;
    bool isWhiteKingChecked = false;
    bool isBlackKingChecked = false;

    bool respectBoardLength(int in_iRow, int in_iColumn) const;
    static bool isCoordinateInVector(const Coordinate& coordTargetPoint, const std::vector<Coordinate>& vectPossibleMoves);



public:
    Board();
    void initializeBoard();

    bool isWhiteKingCheck() const;

    bool isBlackKingCheck() const;

    Piece* getPieceAt(const Coordinate& in_coord) const;

    Piece* getPieceAt(int in_iRow, int in_iColumn) const;

    void clearBoard();

    bool placePiece(int in_iRow, int in_iCol, Piece* in_pPiece);
    bool movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer = Color::WHITE);
    bool isMoveValid(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer = Color::WHITE);
    bool isCheckmated(int in_iStartRow, int in_iStartCol, Color in_colPlayer);
    bool isPat(int in_iStartRow, int in_iStartCol, Color in_colPlayer);
    std::vector<Move> listOfPossibleMoves(Color in_colColor);
    std::vector<Coordinate> possibleMovesForPiece(const Coordinate& in_coordPiece);
    void displayBoard() const;

    std::vector<Coordinate> getMovementsPossibleWithVector(int in_iStartRow, int in_iStartCol, Vector& in_vectMove);

    bool isCaseAttackedByColor(int in_iRow, int in_iCol, Color in_colorToFindAttack) const ;
    bool isVectorsProjectionsAttackingCase(int in_iRow, int in_iColumn, Color in_colorToFindAttack, const Vector* in_tabVectorOfPiece, int in_iNbVector, TypeOfPieceAttack in_typeOfAttackOfTheVector) const;
    Piece* findFirstPieceOnVector(int in_iStartRow, int in_iStartCol, Vector& in_vectMove, int& in_iIndicePieceFound) const;
    static bool doesPieceHaveGoodTypeOfAttack(Piece* in_pPieceToVerifyAttack, TypeOfPieceAttack in_typeOfAttack) ;
    bool isTherePiecesBetweenKingAndRookNotMoving(Vector& in_vect, int in_iRowStart, int in_iColumnStart) const;
    bool isKingInCheck(Color in_kingColor) const;

    Coordinate findKing(Color in_colorToFind) const;
    void promotePawn(int in_iEndRow, int in_iEndCol, Color in_colPlayer, Piece** ppPiece);
    void putNextMoveIfValid(Coordinate& in_coordKing, bool in_isKingInCheck, Coordinate& in_coordNextMove, Piece* in_pPieceToMove, Piece* pPieceFoundOnNextMove, std::vector<Coordinate>& in_vectMoveToFill);
};

#endif //BOARD_H
