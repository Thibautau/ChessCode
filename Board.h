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
    Coordinate m_enPassantPosition;
    bool m_isGameOver = false;
    bool m_isWhiteKingChecked = false;
    bool m_isBlackKingChecked = false;

    bool m_bWhiteKingCanLittleRock = true;
    bool m_bWhiteKingCanBigRock = true;
    bool m_bBlackKingCanLittleRock = true;
    bool m_bBlackKingCanBigRock = true;

    int m_iBlackKingPosition = 60;
    int m_iWhiteKingPosition = 4;
    int m_ipositionEnPassant = -1;

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
    bool movePiece(int in_iStartPosition, int in_iEndPosition, Color in_colPlayer = Color::WHITE, Piece** piece = nullptr,TypePieces promotionType = TypePieces::NONE, int* enPassantPos = nullptr);
    bool movePiece(Color in_colPlayer, const std::string& move, Piece** piece = nullptr,TypePieces promotionType = TypePieces::NONE, int* enPassantPos = nullptr); // overload of the function right above
    void setCastlingRightsForFenNotation(const std::string& castling);
    void setEnPassantPosition(int enPassantPos);
    static int convertToPosition(char col, char row);

    // Getters
    int getEnPassantPosition() const;



    void removeRockPossibility(Color in_colPlayer, int i_columnRook);

    bool isMovementPossible(int in_iStartPosition, int in_iTargetPosition);

    bool isCaseAttackedByColor(int in_iPosition, Color in_colorToFindAttack, std::vector<int>& in_vectPositionPieceFound) const;
    bool isCaseAttackedByAnyColor(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    void findFirstPiecesOnEachRookMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    void findFirstPiecesOnEachBishopMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    void findFirstPiecesOnEachKnightMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;

    bool movePiece(const std::string& move, Color in_colPlayer = Color::WHITE);
    bool movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer = Color::WHITE);
    void possibleMovesForPiece(int in_iPositionToSeeMoves, std::vector<int>& in_vectPossibleMoves);
    void getPieceMovementsPossible(int in_iPositionToFindMovement, int in_iDirectionMovement, int in_iNbOfRepetition, std::vector<int>& in_vectPositionPossible);
    void getAllPossibleMovementsForAPiece(int in_iPositionToFindMovement, std::vector<int>& out_vectDirectionToFill);
    bool putNextMoveIfValid(int in_iNextPosition, Piece* in_pPieceToMove, std::vector<int>& in_vectMoveToFill);

    int getKingPosition(Color in_kingColor) const;
    bool kingCanLittleRock(Color in_kingColor) const;
    bool kingCanBigRock(Color in_kingColor) const;
    bool isKingAttackedAfterMove(Color in_kingColor, Color in_attackerColor) const;
    bool isKingInCheck(Color in_kingColor) const;
    void putOrRemoveKingInCheck(Color in_kingColor, bool in_bPutKingInCheck);

    void promotePawn(Color in_colPlayer, Piece** ppPiece,TypePieces promotionType = TypePieces::NONE);
    /**
     * Does not check if the cases are attacked.
     * @param in_colKing
     * @param in_iDirectionForRock
     * @return
     */
    bool doesKingCanRock(Color in_colKing, int in_iDirectionForRock) const;
    Piece* findFirstPieceOnDirectionThatAttacksInitialPosition(int in_iPosition,int in_iDirection, int in_iNbOfRepetition, int& in_iPositionPieceFound) const;
    Piece* findFirstPieceOnDirection(int in_iPosition,int in_iDirection, int in_iNbOfRepetition, int& in_iPositionPieceFound) const;

    std::vector<std::pair<int, std::vector<int>>> listOfPossibleMovements(Color in_colColor);

    std::vector<std::pair<int, int>> listOfPossibleMoves(Color in_colColor);
    int evaluate(Color in_colPlayer) const;
    bool isGameOver(Color colCurrent_player, Color& out_colWinner);
    bool undoMove(int in_iStartPosition, int in_iEndPosition, Piece* capturedPiece,bool promotion=false,int enPassantPos=-1);

    bool isPromotionMove(int start, int end, Color color);
    int getPieceValue(TypePieces type);
    int evaluateMove(const std::pair<int, int>& move, Color color);





    /* ------------- OLD FUNCTION BELOW ------------- */

    Piece* getPieceAt(int in_iRow, int in_iColumn) const;



    bool placePiece(int in_iRow, int in_iCol, Piece* in_pPiece);
    bool isCheckmated(int in_iStartRow, int in_iStartCol, Color in_colPlayer);
    std::vector<Coordinate> possibleMovesForPiece(const Coordinate& in_coordPiece);
    void displayBoard() const;

    std::vector<Coordinate> getMovementsPossibleWithVector(int in_iStartRow, int in_iStartCol, Vector& in_vectMove);

    bool isCaseAttackedByColor(int in_iRow, int in_iCol, Color in_colorToFindAttack) const;
    bool isVectorsProjectionsAttackingCase(int in_iRow, int in_iColumn, Color in_colorToFindAttack, const Vector* in_tabVectorOfPiece, int in_iNbVector, TypeOfPieceAttack in_typeOfAttackOfTheVector) const;
    Piece* findFirstPieceOnVector(int in_iStartRow, int in_iStartCol, Vector& in_vectMove, int& in_iIndicePieceFound) const;
    static bool doesPieceHaveGoodTypeOfAttack(Piece* in_pPieceToVerifyAttack, TypeOfPieceAttack in_typeOfAttack) ;
    bool isTherePiecesBetweenKingAndRookNotMoving(Vector& in_vect, int in_iRowStart, int in_iColumnStart) const;


    Coordinate findKing(Color in_colorToFind) const;
    void putNextMoveIfValid(Coordinate& in_coordKing, bool in_isKingInCheck, Coordinate& in_coordNextMove, Piece* in_pPieceToMove, Piece* pPieceFoundOnNextMove, std::vector<Coordinate>& in_vectMoveToFill);


private:
    static void convertMoveToPositions(const std::string& move, int& startPos, int& endPos);
};

#endif //BOARD_H
