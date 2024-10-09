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
    Piece* m_tabpiBoard[64]{};
    Piece* m_tabtabpiBoard[8][8]{};
    Coordinate m_enPassantPosition;

    bool m_isWhiteKingChecked = false;
    bool m_isBlackKingChecked = false;

    bool m_bWhiteKingCanLittleRock = true;
    bool m_bWhiteKingCanBigRock = true;
    bool m_bBlackKingCanLittleRock = true;
    bool m_bBlackKingCanBigRock = true;

    int m_iBlackKingPosition = 60;
    int m_iWhiteKingPosition = 4;

    bool respectBoardLength(int in_iRow, int in_iColumn) const;
    static bool isCoordinateInVector(const Coordinate& coordTargetPoint, const std::vector<Coordinate>& vectPossibleMoves);



public:
    Board();
    void initializeBoard();
    void clearBoard();
    bool isWhiteKingCheck() const;
    bool isBlackKingCheck() const;
    static bool isValidPosition(int in_iPosition);

    bool placePiece(const std::string& move, Piece* in_pPiece);
    bool placePiece(int in_iPositionPiece, Piece* in_pPiece);
    Piece* getPieceAt(int  in_iPositionPiece) const;
    Piece* getPieceAt(const std::string& in_sPosition) const;
    bool movePiece(int in_iStartPosition, int in_iEndPosition, Color in_colPlayer = Color::WHITE);
    bool isMovementPossible(int in_iStartPosition, int in_iTargetPosition);
    bool isCaseAttackedByColor(int in_iPosition, Color in_colorToFindAttack, std::vector<int>& in_vectPositionPieceFound) const;
    bool isCaseAttackedByAnyColor(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    void findFirstPiecesOnEachRookMovements(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    void findFirstPiecesOnEachBishopMovements(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    void findFirstPiecesOnEachKnightMovements(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;

    bool movePiece(const std::string& move, Color in_colPlayer = Color::WHITE);
    bool movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer = Color::WHITE);
    void possibleMovesForPiece(int in_iPositionToSeeMoves, std::vector<int>& in_vectPossibleMoves);
    void getPieceMovementsPossible(int in_iPositionToFindMovement, int in_iDirectionMovement, int in_iNbOfRepetition, std::vector<int>& in_vectPositionPossible);
    void getAllPossibleMovementsForAPiece(int in_iPositionToFindMovement, std::vector<int>& out_vectDirectionToFill);
    void putNextMoveIfValid(int in_iNextPosition, Piece* in_pPieceToMove, std::vector<int>& in_vectMoveToFill);

    /**
     * Does not check if the cases are attacked.
     * @param in_colKing
     * @param in_iDirectionForRock
     * @return
     */
    bool doesKingCanRock(Color in_colKing, int in_iDirectionForRock) const;
    Piece* findFirstPieceOnDirection(int in_iPosition,int in_iDirection, int in_iNbOfRepetition, int& in_iPositionPieceFound) const;

    std::vector<std::pair<int, std::vector<int>>> listOfPossibleMovements(Color in_colColor);

    /* ------------- OLD FUNCTION BELOW ------------- */


    Piece* getPieceAt(const Coordinate& in_coord) const;

    Piece* getPieceAt(int in_iRow, int in_iColumn) const;



    bool placePiece(int in_iRow, int in_iCol, Piece* in_pPiece);
    //bool movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer = Color::WHITE);
    bool isMoveValid(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer = Color::WHITE);
    bool isCheckmated(int in_iStartRow, int in_iStartCol, Color in_colPlayer);
    std::vector<Move> listOfPossibleMoves(Color in_colColor);
    //std::vector<Coordinate> possibleMovesForPiece(const Coordinate& in_coordPiece);
    void displayBoard() const;

    std::vector<Coordinate> getMovementsPossibleWithVector(int in_iStartRow, int in_iStartCol, Vector& in_vectMove);

    bool isCaseAttackedByColor(int in_iRow, int in_iCol, Color in_colorToFindAttack) const;
    bool isVectorsProjectionsAttackingCase(int in_iRow, int in_iColumn, Color in_colorToFindAttack, const Vector* in_tabVectorOfPiece, int in_iNbVector, TypeOfPieceAttack in_typeOfAttackOfTheVector) const;
    Piece* findFirstPieceOnVector(int in_iStartRow, int in_iStartCol, Vector& in_vectMove, int& in_iIndicePieceFound) const;
    static bool doesPieceHaveGoodTypeOfAttack(Piece* in_pPieceToVerifyAttack, TypeOfPieceAttack in_typeOfAttack) ;
    bool isTherePiecesBetweenKingAndRookNotMoving(Vector& in_vect, int in_iRowStart, int in_iColumnStart) const;
    bool isKingInCheck(Color in_kingColor) const;

    Coordinate findKing(Color in_colorToFind) const;
    void promotePawn(int in_iEndRow, int in_iEndCol, Color in_colPlayer, Piece** ppPiece);
    void putNextMoveIfValid(Coordinate& in_coordKing, bool in_isKingInCheck, Coordinate& in_coordNextMove, Piece* in_pPieceToMove, Piece* pPieceFoundOnNextMove, std::vector<Coordinate>& in_vectMoveToFill);


private:
    static int convertToPosition(char col, char row);
    static void convertMoveToPositions(const std::string& move, int& startPos, int& endPos);
};

#endif //BOARD_H
