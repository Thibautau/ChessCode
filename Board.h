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



public:
    // Initialisation and Setup Functions
    Board();
    void initializeBoard();
    void clearBoard();
    void setupFromFEN(const std::string& fen);
    void setCastlingRightsForFenNotation(const std::string& castling);

    //Piece Placement and Movement Functions
    bool placePiece(const std::string& move, Piece* in_pPiece);
    bool placePiece(int in_iPositionPiece, Piece* in_pPiece);
    bool placePiece(int in_iRow, int in_iCol, Piece* in_pPiece);
    bool movePiece(int in_iStartPosition, int in_iEndPosition, Color in_colPlayer = Color::WHITE, Piece** piece = nullptr,TypePieces promotionType = TypePieces::NONE, int* enPassantPos = nullptr);
    bool movePiece(Color in_colPlayer, const std::string& move, Piece** piece = nullptr,TypePieces promotionType = TypePieces::NONE, int* enPassantPos = nullptr);
    bool movePiece(const std::string& move, Color in_colPlayer = Color::WHITE, TypePieces promotionType = TypePieces::QUEEN);
    bool movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer = Color::WHITE);

    //Checking and Validation Functions
    bool isWhiteKingCheck() const;
    bool isBlackKingCheck() const;
    bool isMovementPossible(int in_iStartPosition, int in_iTargetPosition);
    bool isCheckmated(int in_iStartRow, int in_iStartCol, Color in_colPlayer);
    bool isKingAttackedAfterMove(Color in_kingColor, Color in_attackerColor) const;
    bool isKingInCheck(Color in_kingColor) const;
    void putOrRemoveKingInCheck(Color in_kingColor, bool in_bPutKingInCheck);
    bool isPromotionMove(int start, int end, Color color);
    bool doesKingCanRock(Color in_colKing, int in_iDirectionForRock) const;
    bool kingCanLittleRock(Color in_kingColor) const;
    bool kingCanBigRock(Color in_kingColor) const;
    static bool doesPieceHaveGoodTypeOfAttack(Piece* in_pPieceToVerifyAttack, TypeOfPieceAttack in_typeOfAttack);

    //Attack and Threat Detection Functions
    bool isCaseAttackedByColor(int in_iPosition, Color in_colorToFindAttack, std::vector<int>& in_vectPositionPieceFound) const;
    bool isCaseAttackedByAnyColor(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    Piece* findFirstPieceOnDirectionThatAttacksInitialPosition(int in_iPosition,int in_iDirection, int in_iNbOfRepetition, int& in_iPositionPieceFound) const;
    Piece* findFirstPieceOnDirection(int in_iPosition,int in_iDirection, int in_iNbOfRepetition, int& in_iPositionPieceFound) const;
    void findFirstPiecesOnEachRookMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    void findFirstPiecesOnEachBishopMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    void findFirstPiecesOnEachKnightMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;

    //Piece Movement Options and Calculations
    void possibleMovesForPiece(int in_iPositionToSeeMoves, std::vector<int>& in_vectPossibleMoves);
    void getPieceMovementsPossible(int in_iPositionToFindMovement, int in_iDirectionMovement, int in_iNbOfRepetition, std::vector<int>& in_vectPositionPossible);
    void getAllPossibleMovementsForAPiece(int in_iPositionToFindMovement, std::vector<int>& out_vectDirectionToFill);
    bool putNextMoveIfValid(int in_iNextPosition, Piece* in_pPieceToMove, std::vector<int>& in_vectMoveToFill);
    std::vector<std::pair<int, std::vector<int>>> listOfPossibleMovements(Color in_colColor);
    std::vector<std::pair<int, int>> listOfPossibleMoves(Color in_colColor);
    bool undoMove(int in_iStartPosition, int in_iEndPosition, Piece* capturedPiece,bool promotion=false,int enPassantPos=-1);

    //Evaluation and Heuristic Functions
    int evaluate(Color in_colPlayer) const;
    int evaluateMove(const std::pair<int, int>& move, Color color);
    int evaluatePawnStructure(int position, Color color) const;
    int evaluateKingSafety(Color color) const;
    int getPieceValue(TypePieces type);

    //Promotion and Castling Functions
    void promotePawn(Color in_colPlayer, Piece** ppPiece,TypePieces promotionType = TypePieces::NONE);
    void removeRockPossibility(Color in_colPlayer, int i_columnRook);

    //Utility and Conversion Functions
    static bool isValidPosition(int in_iPosition);
    int getEnPassantPosition() const;
    void setEnPassantPosition(int pos);
    int getIndexByPiece(TypePieces type, Color color) const;
    std::vector<int> getBoardStateAsVector() const;
    static int convertToPosition(char col, char row);
    static void convertMoveToPositions(const std::string& move, int& startPos, int& endPos);

    //Piece Retrieval Functions
    Piece* getPieceAt(int  in_iPositionPiece) const;
    Piece* getPieceAt(const std::string& in_sPosition) const;
    Piece* getPieceAt(int in_iRow, int in_iColumn) const;

    //Game Status and Check/End Conditions
    int getKingPosition(Color in_kingColor) const;
    bool isGameOver(Color colCurrent_player, Color& out_colWinner);
    void displayBoard() const;
};





#endif //BOARD_H
