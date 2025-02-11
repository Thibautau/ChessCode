//
// Created by Peter on 02/10/2024.
//

#include "Board.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>

//************************ Initialisation and Setup Functions ************************//
Board::Board()
{
    for(int iIndiceRow = 0; iIndiceRow < 64; iIndiceRow++ )
    {
        m_tabpiBoard[iIndiceRow] = nullptr;
    }

    m_bWhiteKingCanLittleRock = true;
    m_bWhiteKingCanBigRock = true;
    m_bBlackKingCanLittleRock = true;
    m_bBlackKingCanBigRock = true;

    m_isWhiteKingChecked = false;
    m_isBlackKingChecked = false;

    m_iBlackKingPosition = 60;
    m_iWhiteKingPosition = 4;

    m_iPreviousMoveInitialPosition = -1;
    m_iPreviousMoveTargetPosition = -1;
}

Board::~Board()
{
    for(int iIndiceRow = 0; iIndiceRow < 64; iIndiceRow++ )
    {
        if (m_tabpiBoard[iIndiceRow] != nullptr)
        {
            delete m_tabpiBoard[iIndiceRow];
            m_tabpiBoard[iIndiceRow] = nullptr;
        }
    }
}

void Board::initializeBoard()
{
    for (int iTabPosition = 8; iTabPosition < 16; ++iTabPosition) {
        placePiece(iTabPosition, new Piece(TypePieces::PAWN, Color::WHITE));
        placePiece(55 + (8 - iTabPosition), new Piece(TypePieces::PAWN, Color::BLACK));
    }

    placePiece(0, new Piece(TypePieces::ROOK, Color::WHITE));
    placePiece(1, new Piece(TypePieces::KNIGHT, Color::WHITE));
    placePiece(2, new Piece(TypePieces::BISHOP, Color::WHITE));
    placePiece(3, new Piece(TypePieces::QUEEN, Color::WHITE));
    placePiece(4, new Piece(TypePieces::KING, Color::WHITE));
    placePiece(5, new Piece(TypePieces::BISHOP, Color::WHITE));
    placePiece(6, new Piece(TypePieces::KNIGHT, Color::WHITE));
    placePiece(7, new Piece(TypePieces::ROOK, Color::WHITE));

    placePiece(56, new Piece(TypePieces::ROOK, Color::BLACK));
    placePiece(57, new Piece(TypePieces::KNIGHT, Color::BLACK));
    placePiece(58, new Piece(TypePieces::BISHOP, Color::BLACK));
    placePiece(59, new Piece(TypePieces::QUEEN, Color::BLACK));
    placePiece(60, new Piece(TypePieces::KING, Color::BLACK));
    placePiece(61, new Piece(TypePieces::BISHOP, Color::BLACK));
    placePiece(62, new Piece(TypePieces::KNIGHT, Color::BLACK));
    placePiece(63, new Piece(TypePieces::ROOK, Color::BLACK));
}

void Board::clearBoard()
{
    for(int iIndiceRow = 0; iIndiceRow < 64; iIndiceRow++ )
    {
        if (m_tabpiBoard[iIndiceRow] != nullptr)
        {
            delete m_tabpiBoard[iIndiceRow];
            m_tabpiBoard[iIndiceRow] = nullptr;
        }
    }

    m_isWhiteKingChecked = false;
    m_isBlackKingChecked = false;

    m_bWhiteKingCanLittleRock = true;
    m_bWhiteKingCanBigRock = true;
    m_bBlackKingCanLittleRock = true;
    m_bBlackKingCanBigRock = true;

    m_isWhiteKingChecked = false;
    m_isBlackKingChecked = false;

    m_iBlackKingPosition = 60;
    m_iWhiteKingPosition = 4;

    m_iPreviousMoveInitialPosition = -1;
    m_iPreviousMoveTargetPosition = -1;

    for (int i = 0; i < 4; i++)
    {
        m_itabIndexRockPrivilegeLostOnMove[i] = -1;
    }
}

void Board::setupFromFEN(const std::string& fen) {
    clearBoard();

    int row = 7;
    int col = 0;

    for (char c : fen)
    {
        if (c == ' ')
        {
            break;
        }
        if (c == '/')
        {
            row--;
            col = 0;
        }
        else if (isdigit(c))
        {
            int emptySquares = c - '0';
            col += emptySquares;
        }
        else
        {
            Color color = isupper(c) ? Color::WHITE : Color::BLACK;
            TypePieces pieceType = Piece::charToPieceType(tolower(c));
            if (col >= 0 && col < 8)
            {
                if(pieceType == TypePieces::KING)
                {
                    if(color == Color::WHITE)
                    {
                        m_iWhiteKingPosition = (row * 8) + col;
                    }
                    else
                    {
                        m_iBlackKingPosition = (row * 8) + col;
                    }
                }

                placePiece(row, col, new Piece(pieceType, color));
                col++;
            }
            else
            {
                std::cerr << "Column overflow at row " << row << std::endl;
            }
        }
    }

    std::vector<int> uselessVector;
    m_isWhiteKingChecked = isCaseAttackedByColor(m_iWhiteKingPosition, Color::BLACK, uselessVector);
    uselessVector.clear();
    m_isBlackKingChecked = isCaseAttackedByColor(m_iBlackKingPosition, Color::WHITE, uselessVector);
}

void Board::setCastlingRightsForFenNotation(const std::string& castling)
{
    m_bWhiteKingCanLittleRock = (castling.find('K') != std::string::npos);
    m_bWhiteKingCanBigRock = (castling.find('Q') != std::string::npos);
    m_bBlackKingCanLittleRock = (castling.find('k') != std::string::npos);
    m_bBlackKingCanBigRock = (castling.find('q') != std::string::npos);
}

//************************************************************************************//

//*********************** Piece Placement and Movement Functions *********************//

bool Board::placePiece(const std::string& move, Piece* in_pPiece)
{
    int iStartPos = convertToPosition(move[0], move[1]);
    return placePiece(iStartPos, in_pPiece);
}

bool Board::placePiece(int in_iRow, int in_iCol, Piece* in_pPiece)
{
    if(in_iRow < 0 || in_iRow >= 8 || in_iCol < 0 || in_iCol >= 8)
    {
        return false;
    }
    m_tabpiBoard[in_iRow*8+in_iCol] = in_pPiece;
    return true;
}

bool Board::placePiece(int in_iPositionPiece, Piece* in_pPiece)
{
    if(! isValidPosition(in_iPositionPiece))
    {
        return false;
    }

    if(in_pPiece != nullptr && in_pPiece->getTypePiece() == TypePieces::KING) // If we place a king, we update its coordinates
    {
        if(in_pPiece->getColor() == Color::WHITE)
        {
            m_iWhiteKingPosition = in_iPositionPiece;
        }
        else
        {
            m_iBlackKingPosition = in_iPositionPiece;
        }
    }

    m_tabpiBoard[in_iPositionPiece] = in_pPiece;
    return true;
}

bool Board::movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer)
{
    return movePiece((in_iStartRow * 8) + in_iStartCol, (in_iEndRow * 8) + in_iEndCol, in_colPlayer);
}

bool Board::movePiece(const std::string& move, Color in_colPlayer, TypePieces promotionType)
{
    int iStartPosition, iEndPosition;
    convertMoveToPositions(move, iStartPosition, iEndPosition);
    return movePiece(iStartPosition, iEndPosition, in_colPlayer, nullptr, promotionType);
}

bool Board::movePiece(Color in_colPlayer, const std::string& move, Piece** piece,TypePieces promotionType, int* enPassantPos)
{
    int iStartPosition, iEndPosition;
    convertMoveToPositions(move, iStartPosition, iEndPosition);
    return movePiece(iStartPosition, iEndPosition, in_colPlayer, piece, promotionType, enPassantPos);
}

bool Board::movePiece(int in_iStartPosition, int in_iEndPosition, Color in_colPlayer,Piece** piece, TypePieces promotionType, int* enPassantPos)
{
    Piece* pPiece = getPieceAt(in_iStartPosition);

    if(pPiece == nullptr || pPiece->getColor() != in_colPlayer) //If the player try to move a piece of another color, return false
    {
        return false;
    }

    //Color colPieceToMove = pPiece->getColor();
    //Color colEnemyPieceToMove = pPiece->getEnemyColor();

    Color colEnemy = pPiece->getEnemyColor();

    if(enPassantPos) // Optional argument so we have to verify if it exists
    {
        *enPassantPos = m_ipositionEnPassant;
    }

    if(in_iStartPosition == 30 && in_iEndPosition == 58) {
        int a = 2;
    }

    //If movement isn't possible we stop the move
    if (!isMovementPossible(in_iStartPosition, in_iEndPosition)) {
        return false;
    }

    { // We reset those indexs because we want to update those values with this move
        m_itabIndexRockPrivilegeLostOnMove[0] = -1;
        m_itabIndexRockPrivilegeLostOnMove[1] = -1;
        m_itabIndexRockPrivilegeLostOnMove[2] = -1;
        m_itabIndexRockPrivilegeLostOnMove[3] = -1;
    }

    bool wasEnPassant = false;

    //Gestion des cas spécifique du pion
    if(pPiece->getTypePiece() == TypePieces::PAWN) {

        //Capture enPassant
        if(in_iEndPosition==m_ipositionEnPassant) {
            int direction = (in_colPlayer == Color::WHITE) ? -1 : 1;
            if(piece) {
                *piece = getPieceAt(m_ipositionEnPassant+direction*8);
            }
            placePiece(m_ipositionEnPassant+direction*8,nullptr);
        }

        // Set en passant position
        if(abs(in_iEndPosition - in_iStartPosition) == 16) {
            m_ipositionEnPassant = in_iStartPosition + ((in_colPlayer == Color::WHITE) ? 8 : -8);
            if (enPassantPos) {*enPassantPos = m_ipositionEnPassant;}
            wasEnPassant = true;
        }

        //Promotion du pion
        if(in_iEndPosition/8 == 0 || in_iEndPosition/8 == 7) {
            promotePawn(in_colPlayer, &pPiece,promotionType);
        }
    }

    //Gestion cas spécifique du roi
    if(pPiece->getTypePiece() == TypePieces::KING) // If the king rocked (move of length 2, we move the rook)
    {
        int delta = in_iEndPosition - in_iStartPosition;
        bool bKingWentRightForRock = (delta == 2);
        bool bKingWentLeftForRock = (delta == -2);

        (in_colPlayer == Color::WHITE ? m_iWhiteKingPosition : m_iBlackKingPosition) = in_iEndPosition;

        int rookStartPos = bKingWentRightForRock? in_iStartPosition + 7 - (in_iStartPosition % 8) : in_iStartPosition - (in_iStartPosition % 8);
        int rookEndPos = bKingWentRightForRock? in_iStartPosition + 1 : in_iStartPosition - 1;

        Piece* pRookForRock = nullptr;
        pRookForRock = getPieceAt(rookStartPos);
        if((bKingWentRightForRock || bKingWentLeftForRock) && pRookForRock != nullptr)
        {
            bool bCouldPLacePiece = placePiece(rookEndPos, pRookForRock);
            if(! bCouldPLacePiece){
                return false;
            }
            m_tabpiBoard[rookStartPos] = nullptr; // La dernière case de la ligne
        }
        removeRockPossibility(in_colPlayer, in_iStartPosition % 8);
    }

    //Gestion cas spécifique de la tour
    if (pPiece->getTypePiece() == TypePieces::ROOK) {
        removeRockPossibility(in_colPlayer, in_iStartPosition % 8);
    }

    if(!wasEnPassant && m_ipositionEnPassant !=-1) {
        m_ipositionEnPassant = -1;
    }

    if(piece && !wasEnPassant && getPieceAt(in_iEndPosition) != nullptr) // getPieceAt(in_iEndPosition) != nullptr because we don't want to get here when we are doing enPassant (eating)
    {
        *piece = getPieceAt(in_iEndPosition);
    }
    placePiece(in_iEndPosition, pPiece);
    m_tabpiBoard[in_iStartPosition] = nullptr;

    // If we were able to move, it means that we are not in check anymore
    putOrRemoveKingInCheck(pPiece->getColor(), false);
    if (isKingAttackedAfterMove(colEnemy, in_colPlayer)) {
        putOrRemoveKingInCheck(colEnemy, true);
    }

    m_iPreviousMoveInitialPosition = in_iStartPosition;
    m_iPreviousMoveTargetPosition = in_iEndPosition;
    return true;
}

//************************************************************************************//


//************************** Checking and Validation Functions **********************//

bool Board::isWhiteKingCheck() const
{
    return m_isWhiteKingChecked;
}

bool Board::isBlackKingCheck() const
{
    return m_isBlackKingChecked;
}

//@TODO Enlever le tableau dynamique vector et le remplacer par autre chose?
bool Board::isMovementPossible(int in_iStartPosition, int in_iTargetPosition)
{
    if(! isValidPosition(in_iStartPosition) || ! isValidPosition(in_iTargetPosition))
    {
        return false;
    }

    Piece* pPieceToSeeValidMove = getPieceAt(in_iStartPosition);
    Piece* pPieceAtTargetPosition = getPieceAt(in_iTargetPosition);
    if(pPieceToSeeValidMove == nullptr)
    {
        return false;
    }
    if(pPieceAtTargetPosition != nullptr && pPieceAtTargetPosition->getColor() == pPieceToSeeValidMove->getColor()) // Si les 2 pièces ont la même couleur
    {
        return false;
    }

    std::vector<int> itabValidPositions;
    possibleMovesForPiece(in_iStartPosition, itabValidPositions);

    if (std::ranges::find(itabValidPositions.begin(), itabValidPositions.end(), in_iTargetPosition) != itabValidPositions.end()) {
        return true;
    }

    return false;
}

//@TODO Il y a une consition impossible, why????
bool Board::isCheckmated(int in_iStartRow, int in_iStartCol, Color in_colPlayer) {
    m_isWhiteKingChecked = true;
    if(in_colPlayer == Color::WHITE) {
        if(m_isWhiteKingChecked == false) {
            return false;
        }
    }
    else if(in_colPlayer == Color::BLACK) {
        if(m_isBlackKingChecked == false) {
            return false;
        }
    }
    std::vector<Coordinate> vectPossibleMovesForKing /*= possibleMovesForPiece(Coordinate(in_iStartRow, in_iStartCol))*/;
    if(!vectPossibleMovesForKing.empty()) {
        return false;
    }
    return true;
}

//@TODO Voir pour virer les vector et mettre des tableaux statiques
bool Board::isKingAttackedAfterMove(Color in_kingColor, Color in_attackerColor) const
{
    int iKingPosition = getKingPosition(in_kingColor);

    std::vector<int> vectAttackingPieces;
    if(isCaseAttackedByColor(iKingPosition, in_attackerColor, vectAttackingPieces))
    {
        return true;
    }
    return false;
}

bool Board::isKingInCheck(Color in_kingColor) const
{
    return in_kingColor == Color::WHITE ? m_isWhiteKingChecked : m_isBlackKingChecked;
}

//@TODO Proposition de refactor, à valider sinon reprendre l'ancien code
void Board::putOrRemoveKingInCheck(Color in_kingColor, bool in_bPutKingInCheck)
{
    if (in_kingColor == Color::WHITE || in_kingColor == Color::BLACK) {
        bool& kingCheck = (in_kingColor == Color::WHITE) ? m_isWhiteKingChecked : m_isBlackKingChecked;
        kingCheck = in_bPutKingInCheck;
    }
    /*switch (in_kingColor) {
        case Color::WHITE:
            m_isWhiteKingChecked = in_bPutKingInCheck;
        break;
        case Color::BLACK:
            m_isBlackKingChecked = in_bPutKingInCheck;
        break;
        default:
            return;
    }*/
}

//@TODO Proposition de refactor, à valider sinon reprendre l'ancien code
bool Board::isPromotionMove(int start, int end, Color color) {
    Piece* piece = getPieceAt(start);
    return piece && piece->getTypePiece() == TypePieces::PAWN && ((color == Color::WHITE && end / 8 == 7) || (color == Color::BLACK && end / 8 == 0));
    /*if (piece && piece->getTypePiece() == TypePieces::PAWN) {
        int endRow = end / 8;
        if ((color == Color::WHITE && endRow == 7) || (color == Color::BLACK && endRow == 0)) {
            return true;
        }
    }
    return false;*/
}

//@TODO Proposition de refactor, à valider sinon reprendre l'ancien code
// DO not check if the cases are attacked
bool Board::doesKingCanRock(Color in_colKing, int in_iDirectionForRock) const
{
    //int iKingPosition;


    if ((in_colKing == Color::WHITE && m_isWhiteKingChecked) || (in_colKing == Color::BLACK && m_isBlackKingChecked) || (in_colKing==Color::NONE))  {
        return false;
    }

    int iKingPosition = (in_colKing == Color::WHITE) ? m_iWhiteKingPosition : m_iBlackKingPosition;


    /*switch(in_colKing)
    {
        case Color::WHITE:
            if(m_isWhiteKingChecked)
            {
                return false;
            }
        iKingPosition = m_iWhiteKingPosition;
        break;
        case Color::BLACK:
            if(m_isBlackKingChecked)
            {
                return false;
            }
        iKingPosition = m_iBlackKingPosition;
        break;
        default:
            return false;
    }*/

    int iPositionPieceFound = -1;
    Piece* pPieceFound = findFirstPieceOnDirection(iKingPosition, in_iDirectionForRock, 7, iPositionPieceFound);
    if(pPieceFound == nullptr)
    {
        return false;
    }

    if(pPieceFound->getTypePiece() == TypePieces::ROOK && iPositionPieceFound != -1)
    {
        if ((in_colKing == Color::WHITE && ((iPositionPieceFound == 7 && m_bWhiteKingCanLittleRock) || iPositionPieceFound == 0 && m_bWhiteKingCanBigRock)) ||
            (in_colKing == Color::BLACK && ((iPositionPieceFound == 56 && m_bBlackKingCanLittleRock) || iPositionPieceFound == 63 && m_bBlackKingCanBigRock)))  {
            return true;
        }
        return false;

        switch(in_colKing)
        {
            case Color::WHITE:
                if(iPositionPieceFound == 7 && m_bWhiteKingCanLittleRock)
                {
                    return true;
                }
                else if (iPositionPieceFound == 0 && m_bWhiteKingCanBigRock)
                {
                    return true;
                }
            break;
            case Color::BLACK:
                if(iPositionPieceFound == 56 && m_bBlackKingCanLittleRock)
                {
                    return true;
                }
                else if (iPositionPieceFound == 63 && m_bBlackKingCanBigRock)
                {
                    return true;
                }
            break;
            default:
                return false;
        }
    }
    return false;
}

//@TODO Proposition de refactor, à valider sinon reprendre l'ancien code
bool Board::kingCanLittleRock(Color in_kingColor) const
{
    return (in_kingColor==Color::WHITE) ? m_bWhiteKingCanLittleRock : m_bBlackKingCanLittleRock;
    /*switch (in_kingColor)
    {
        case Color::WHITE:
            return m_bWhiteKingCanLittleRock;
        case Color::BLACK:
            return m_bBlackKingCanLittleRock;
        default:
            return false;
    }*/
}

//@TODO Proposition de refactor, à valider sinon reprendre l'ancien code
bool Board::kingCanBigRock(Color in_kingColor) const
{
    return (in_kingColor==Color::WHITE) ? m_bWhiteKingCanBigRock : m_bBlackKingCanBigRock;
    /*switch (in_kingColor)
    {
        case Color::WHITE:
            return m_bWhiteKingCanBigRock;
        case Color::BLACK:
            return m_bBlackKingCanBigRock;
        default:
            return false;
    }*/
}

//************************************************************************************//

//************************* Attack and Threat Detection Functions ********************//

//@TODO Je suis sûr on peux gérer la boucle plus simplement
bool Board::isCaseAttackedByColor(int in_iPosition, Color in_colorToFindAttack, std::vector<int>& in_vectPositionPieceFound) const
{
    if(! isValidPosition(in_iPosition))
    {
        return false;
    }

    bool bCaseAttacked = isCaseAttackedByAnyColor(in_iPosition, in_vectPositionPieceFound);
    if(! bCaseAttacked) // If it is not attacked
    {
        return false;
    }

    for (int iPositionPieceFound: in_vectPositionPieceFound)
    {
        Piece* pPieceFound = getPieceAt(iPositionPieceFound);
        if(pPieceFound != nullptr)
        {
            if(pPieceFound->getColor() == in_colorToFindAttack)
            {
                return true;
            }
        }
    }

    return false;
}

//@TODO Les vecteurs sont surements un problème
bool Board::isCaseAttackedByAnyColor(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const
{
    if(! isValidPosition(in_iPosition))
    {
        return false;
    }

    findFirstPiecesOnEachKnightMovementsThatAttacksInitialPosition(in_iPosition, in_vectPositionPieceFound);
    findFirstPiecesOnEachBishopMovementsThatAttacksInitialPosition(in_iPosition, in_vectPositionPieceFound);
    findFirstPiecesOnEachRookMovementsThatAttacksInitialPosition(in_iPosition, in_vectPositionPieceFound);

    return !in_vectPositionPieceFound.empty();
}

void Board::getAllPiecesEatableByAColor(Color in_playerColor, std::pair<int, int> out_tabMovesToEatPieces[112], int& out_moveCount)
{
    out_moveCount = 0;
    Color enemyColor = Piece::getEnemyColor(in_playerColor);

    int iPositionPawnNextToEnPassant[2] = {-1, -1};
    getEnemyPawnNextToEnPassantPawn(iPositionPawnNextToEnPassant);

    for (int iPosition = 0; iPosition < 64; ++iPosition) {
        Piece* pPiece = getPieceAt(iPosition);
        if (pPiece != nullptr && pPiece->getColor() == in_playerColor) {
            std::vector<int> validMoves;
            possibleMovesForPiece(iPosition, validMoves);
            for (int targetPos : validMoves)
            {
                Piece* pPieceToEat = getPieceAt(targetPos);
                if(pPiece->getTypePiece() == TypePieces::PAWN && ((iPositionPawnNextToEnPassant[0] == iPosition || iPositionPawnNextToEnPassant[1] == iPosition) && targetPos == m_ipositionEnPassant))
                {
                    out_tabMovesToEatPieces[out_moveCount++] = std::make_pair(iPosition, targetPos);
                }
                else if (out_moveCount < 128 && pPieceToEat != nullptr && pPieceToEat->getColor() == enemyColor) {
                    out_tabMovesToEatPieces[out_moveCount++] = std::make_pair(iPosition, targetPos);
                }
            }
        }
    }
}



//@TODO Proposition de refactor, à valider sinon reprendre l'ancien code
Piece* Board::findFirstPieceOnDirectionThatAttacksInitialPosition(int in_iPosition, int in_iDirection, int in_iNbOfRepetition, int& in_iPositionPieceFound) const {

    if(! isValidPosition(in_iPosition) || in_iNbOfRepetition <= 0)
    {
        return nullptr;
    }

    Piece* pPieceFound = findFirstPieceOnDirection(in_iPosition, in_iDirection, in_iNbOfRepetition, in_iPositionPieceFound);

    if(pPieceFound == nullptr || in_iPositionPieceFound == -1 || !pPieceFound->doesPieceMoveInDirection(-in_iDirection)) {
        in_iPositionPieceFound = -1;
        return nullptr;
    }

    int iPositionDifferenceAbsolute = std::abs(in_iPosition - in_iPositionPieceFound);
    int iPositionDifference = in_iPositionPieceFound - in_iPosition;

    if (pPieceFound->getTypePiece() == TypePieces::PAWN) {
        if (iPositionDifferenceAbsolute == 7 || iPositionDifferenceAbsolute == 9) {
            return pPieceFound;
        }
        in_iPositionPieceFound = -1;
        return nullptr;
    }

    if (pPieceFound->getTypePiece() == TypePieces::KING) {
        if(in_iPosition%8 == 7 && (iPositionDifference == -7 || iPositionDifference == 9)) // SI on est sur la colonne à droite, le roi ne peut pas aller en diagonale à droite (dépassement)
        {
            in_iPositionPieceFound = -1;
            return nullptr;
        }
        if (in_iPosition%8 == 0 && (iPositionDifference == 7 || iPositionDifference == -9)) // SI on est sur la colonne à droite, le roi ne peut pas aller en diagonale à gauche (dépassement)
        {
            in_iPositionPieceFound = -1;
            return nullptr;
        }

        if (iPositionDifferenceAbsolute == 1 || iPositionDifferenceAbsolute == 7 || iPositionDifferenceAbsolute == 8 || iPositionDifferenceAbsolute == 9) {
            return pPieceFound;
        }
        in_iPositionPieceFound = -1;
        return nullptr;
    }

    return pPieceFound;
}

Piece* Board::findFirstPieceOnDirection(int in_iPosition,int in_iDirection, int in_iNbOfRepetition, int& in_iPositionPieceFound) const {

    if(! isValidPosition(in_iPosition) || in_iNbOfRepetition <= 0)
    {
        return nullptr;
    }

    in_iNbOfRepetition++;

    int iNextPosition = in_iPosition;

    for(int iNbMovement = 1; iNbMovement < in_iNbOfRepetition; iNbMovement++) {
        if(Piece::isNextPositionNotOutOfBoard(in_iDirection, iNextPosition))
        {
            iNextPosition = iNextPosition + in_iDirection;
        }
        else
        {
            in_iPositionPieceFound = -1;
            return nullptr;
        }


        Piece* pPieceFound = getPieceAt(iNextPosition);
        if(pPieceFound != nullptr)
        {
            in_iPositionPieceFound = iNextPosition;
            return pPieceFound;
        }
    }

    in_iPositionPieceFound = -1;
    return nullptr;
}

void Board::findFirstPiecesOnEachRookMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const
{
    if(! isValidPosition(in_iPosition))
    {
        return;
    }

    int iNbOfRepetition, iNbMovements;
    int iNextMove = in_iPosition;
    const int* itabRookMoves = Piece::getRookMoves(iNbOfRepetition, iNbMovements);

    for (int iIndiceMove = 0; iIndiceMove < iNbMovements; ++iIndiceMove)
    {
        iNextMove = iNextMove + itabRookMoves[iIndiceMove];

        int iPieceFoundPosition = -1;
        Piece* pFirstPieceFound = findFirstPieceOnDirectionThatAttacksInitialPosition(in_iPosition, itabRookMoves[iIndiceMove], iNbOfRepetition, iPieceFoundPosition);
        if(pFirstPieceFound != nullptr && iPieceFoundPosition != -1)
        {
            in_vectPositionPieceFound.push_back(iPieceFoundPosition);
        }
    }
}

void Board::findFirstPiecesOnEachBishopMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const
{
    if(! isValidPosition(in_iPosition))
    {
        return;
    }

    int iNbOfRepetition, iNbMovements;
    const int* itabBishopMoves = Piece::getBishopMoves(iNbOfRepetition, iNbMovements);

    for (int iIndiceMove = 0; iIndiceMove < iNbMovements; ++iIndiceMove)
    {

        int iPieceFoundPosition = -1;
        Piece* pFirstPieceFound = findFirstPieceOnDirectionThatAttacksInitialPosition(in_iPosition, itabBishopMoves[iIndiceMove], iNbOfRepetition, iPieceFoundPosition);
        if(pFirstPieceFound != nullptr && iPieceFoundPosition != -1)
        {
            in_vectPositionPieceFound.push_back(iPieceFoundPosition);
        }
    }
}

void Board::findFirstPiecesOnEachKnightMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const
{
    if(! isValidPosition(in_iPosition))
    {
        return;
    }

    int iNbOfRepetition, iNbMovements;
    const int* itabKnightMoves = Piece::getKnightMoves(iNbOfRepetition, iNbMovements);

    for (int iIndiceMove = 0; iIndiceMove < iNbMovements; ++iIndiceMove)
    {
        int iPieceFoundPosition = -1;
        Piece* pFirstPieceFound = findFirstPieceOnDirectionThatAttacksInitialPosition(in_iPosition, itabKnightMoves[iIndiceMove], iNbOfRepetition, iPieceFoundPosition);
        if(pFirstPieceFound != nullptr && iPieceFoundPosition != -1)
        {
            in_vectPositionPieceFound.push_back(iPieceFoundPosition);
        }
    }
}

//************************************************************************************//

//********************** Piece Movement Options and Calculations ********************//

void Board::possibleMovesForPiece(int in_iPositionToSeeMoves, std::vector<int>& in_vectPossibleMoves)
{
    Piece* pPieceToSeeMoves = getPieceAt(in_iPositionToSeeMoves);
    if(pPieceToSeeMoves == nullptr)
    {
        return;
    }

    int iUseless, iUseless2;

    Color colPieceToSeeMoves = pPieceToSeeMoves->getColor();
    TypePieces typePieceToSeeMoves = pPieceToSeeMoves->getTypePiece();
    m_tabpiBoard[in_iPositionToSeeMoves] = nullptr;
    switch (typePieceToSeeMoves) {
        case TypePieces::PAWN: {
            int direction = (colPieceToSeeMoves == Color::WHITE) ? 1 : -1;
            int startRow = (colPieceToSeeMoves == Color::WHITE) ? 1 : 6;

            // Normal moves
            int forwardMove = in_iPositionToSeeMoves + direction * 8;
            if (isValidPosition(forwardMove) && m_tabpiBoard[forwardMove] == nullptr) {
                putNextMoveIfValid(forwardMove, pPieceToSeeMoves, in_vectPossibleMoves);
            }

            int twoSteps = in_iPositionToSeeMoves + (direction * 16);
            if (in_iPositionToSeeMoves/8 == startRow) {
                if (isValidPosition(twoSteps) && m_tabpiBoard[forwardMove] == nullptr && m_tabpiBoard[twoSteps] == nullptr) {
                    putNextMoveIfValid(twoSteps, pPieceToSeeMoves, in_vectPossibleMoves);
                }
            }

            // Capture moves
            int captureLeft = in_iPositionToSeeMoves + direction * 8 - 1;
            int captureRight = in_iPositionToSeeMoves + direction * 8 + 1;
            if (isValidPosition(captureLeft) && in_iPositionToSeeMoves % 8 != 0 && m_tabpiBoard[captureLeft] != nullptr && m_tabpiBoard[captureLeft]->getColor() != colPieceToSeeMoves) {
                putNextMoveIfValid(captureLeft, pPieceToSeeMoves, in_vectPossibleMoves);
            }
            if (isValidPosition(captureRight) && in_iPositionToSeeMoves % 8 != 7 && m_tabpiBoard[captureRight] != nullptr && m_tabpiBoard[captureRight]->getColor() != colPieceToSeeMoves) {
                putNextMoveIfValid(captureRight, pPieceToSeeMoves, in_vectPossibleMoves);
            }

            int iPositionPawnEnPassant = m_ipositionEnPassant + (8 * direction * -1);
            Piece* pPiecePawnEnPassant = getPieceAt(iPositionPawnEnPassant);
            if(m_ipositionEnPassant!=-1 && pPiecePawnEnPassant != nullptr && pPiecePawnEnPassant->getColor() != colPieceToSeeMoves && pPiecePawnEnPassant->getTypePiece() == TypePieces::PAWN) // Verify that it is not a pawn of the same color
            {
                if(captureLeft==m_ipositionEnPassant) {
                    putNextMoveIfValid(captureLeft, pPieceToSeeMoves, in_vectPossibleMoves);
                }
                if(captureRight == m_ipositionEnPassant) {
                    putNextMoveIfValid(captureRight, pPieceToSeeMoves, in_vectPossibleMoves);
                }
            }
            break;
        }
        case TypePieces::ROOK: {
            int repetitions, numDirections;
            int* directions = Piece::getRookMoves(repetitions, numDirections);

            for (int dir = 0; dir < numDirections; ++dir) {
                for (int rep = 1; rep <= repetitions; ++rep) {
                    int nextPosition = in_iPositionToSeeMoves + directions[dir] * rep;

                    // Utilisation de la fonction isNextPositionValid
                    if (!pPieceToSeeMoves->isNextPositionValid(directions[dir], in_iPositionToSeeMoves, nextPosition)) {
                        break;
                    }

                    if (m_tabpiBoard[nextPosition] == nullptr) {
                        putNextMoveIfValid(nextPosition, pPieceToSeeMoves, in_vectPossibleMoves);
                    } else {
                        if (m_tabpiBoard[nextPosition]->getColor() != colPieceToSeeMoves) {
                            putNextMoveIfValid(nextPosition, pPieceToSeeMoves, in_vectPossibleMoves);
                        }
                        break;
                    }
                }
            }
            break;
        }
        case TypePieces::KNIGHT: {
            const int* knightMoves = Piece::getKnightMoves(iUseless, iUseless2);
            int numMoves = 8;

            for (int i = 0; i < numMoves; ++i) {
                int nextPosition = in_iPositionToSeeMoves + knightMoves[i];

                if (pPieceToSeeMoves->isNextPositionValid(knightMoves[i], in_iPositionToSeeMoves, nextPosition)) {
                    if (m_tabpiBoard[nextPosition] == nullptr || m_tabpiBoard[nextPosition]->getColor() != colPieceToSeeMoves) {
                        putNextMoveIfValid(nextPosition, pPieceToSeeMoves, in_vectPossibleMoves);
                    }
                }
            }
            break;
        }
        case TypePieces::BISHOP: {
            int repetitions, numDirections;
            int* directions = Piece::getBishopMoves(repetitions, numDirections);

            for (int dir = 0; dir < numDirections; ++dir) {
                for (int rep = 1; rep <= repetitions; ++rep) {
                    int nextPosition = in_iPositionToSeeMoves + directions[dir] * rep;

                    if (!pPieceToSeeMoves->isNextPositionValid(directions[dir], in_iPositionToSeeMoves, nextPosition) ){
                        break;
                    }

                    if (m_tabpiBoard[nextPosition] == nullptr) {
                        putNextMoveIfValid(nextPosition, pPieceToSeeMoves, in_vectPossibleMoves);
                    } else {
                        if (m_tabpiBoard[nextPosition]->getColor() != colPieceToSeeMoves) {
                            putNextMoveIfValid(nextPosition, pPieceToSeeMoves, in_vectPossibleMoves);
                        }
                        break;
                    }
                }
            }
            break;
        }
        case TypePieces::QUEEN: {
            int repetitions, numDirections;
            int* directions = Piece::getQueenMoves(repetitions, numDirections);

            for (int dir = 0; dir < numDirections; ++dir) {
                for (int rep = 1; rep <= repetitions; ++rep) {
                    int nextPosition = in_iPositionToSeeMoves + directions[dir] * rep;

                    if (!pPieceToSeeMoves->isNextPositionValid(directions[dir], in_iPositionToSeeMoves, nextPosition)) {
                        break;
                    }

                    if (m_tabpiBoard[nextPosition] == nullptr) {
                        putNextMoveIfValid(nextPosition, pPieceToSeeMoves, in_vectPossibleMoves);
                    } else {
                        if (m_tabpiBoard[nextPosition]->getColor() != colPieceToSeeMoves) {
                            putNextMoveIfValid(nextPosition, pPieceToSeeMoves, in_vectPossibleMoves);
                        }
                        break;
                    }
                }
            }
            break;
        }
        case TypePieces::KING: {
            int iNbRepetitions, iNbMovements;
            const int* kingMoves = Piece::getKingMoves(iNbRepetitions, iNbMovements);
            int numMoves = 8;
            bool bLittleRockPossible = false;
            bool bBigRockPossible = false;

            for (int i = 0; i < numMoves; ++i) {
                int nextPosition = in_iPositionToSeeMoves + kingMoves[i];

                if (pPieceToSeeMoves->isNextPositionValid(kingMoves[i], in_iPositionToSeeMoves, nextPosition)) {
                    if (m_tabpiBoard[nextPosition] == nullptr || m_tabpiBoard[nextPosition]->getColor() != colPieceToSeeMoves) {
                        bool bValidMove = putNextMoveIfValid(nextPosition, pPieceToSeeMoves, in_vectPossibleMoves);
                        if(bValidMove && kingMoves[i] == 1 && m_tabpiBoard[nextPosition] == nullptr) // To not repeat computation for the rock juste after
                        {
                            bLittleRockPossible = true;
                        }
                        else if(bValidMove && kingMoves[i] == -1 && m_tabpiBoard[nextPosition] == nullptr)
                        {
                            bBigRockPossible = true;
                        }
                    }
                }
            }

            bool bIsKingInCheck = isKingInCheck(colPieceToSeeMoves);
            if(bBigRockPossible && kingCanBigRock(colPieceToSeeMoves) && ! bIsKingInCheck)
            {
                const int* iBigRockMoves = Piece::getBigRockMoves(iNbRepetitions, iNbMovements);

                int nextPosition = in_iPositionToSeeMoves + iBigRockMoves[0];

                if (pPieceToSeeMoves->isRockNextPositionValid(iBigRockMoves[0], in_iPositionToSeeMoves, nextPosition))
                {
                    if (m_tabpiBoard[nextPosition] == nullptr)
                    {
                        putNextMoveIfValid(nextPosition, pPieceToSeeMoves, in_vectPossibleMoves);
                    }
                }
            }
            if (bLittleRockPossible && kingCanLittleRock(colPieceToSeeMoves) && ! bIsKingInCheck) {
                const int* iBigRockMoves = Piece::getLittleRockMoves(iNbRepetitions, iNbMovements);

                int nextPosition = in_iPositionToSeeMoves + iBigRockMoves[0];

                if (pPieceToSeeMoves->isRockNextPositionValid(iBigRockMoves[0], in_iPositionToSeeMoves, nextPosition))
                {
                    if (m_tabpiBoard[nextPosition] == nullptr)
                    {
                        putNextMoveIfValid(nextPosition, pPieceToSeeMoves, in_vectPossibleMoves);
                    }
                }
            }
            break;
        }
        default:
            break;
    }
    m_tabpiBoard[in_iPositionToSeeMoves] = pPieceToSeeMoves;
}

void Board::getPieceMovementsPossible(int in_iPositionToFindMovement, int in_iDirectionMovement, int in_iNbOfRepetition, std::vector<int>& in_vectPositionPossible)
{
    if(! isValidPosition(in_iPositionToFindMovement) || in_iNbOfRepetition <= 0)
    {
        return;
    }

    in_iNbOfRepetition++; // It is a local variable, it doesn't affect the one who calls this function
    Piece* pPieceToFindMovement = getPieceAt(in_iPositionToFindMovement);
    TypePieces typePieceToFindMovement = pPieceToFindMovement->getTypePiece();
    Color colPieceToFindMovement = pPieceToFindMovement->getColor();
    int iRowPieceToFindMovement = in_iPositionToFindMovement / 8;
    int iColumnPieceToFindMovement = in_iPositionToFindMovement % 8;

    int iNextPosition = in_iPositionToFindMovement;

    for(int iNbMovement = 1; iNbMovement < in_iNbOfRepetition; iNbMovement++)
    {
        iNextPosition = iNextPosition + in_iDirectionMovement;
        int iNextRow = iNextPosition / 8;
        int iNextColumn = iNextPosition % 8;

        int iRowDifference = std::abs(iRowPieceToFindMovement - iNextRow);
        int iColumnDifference = std::abs(iColumnPieceToFindMovement - iNextColumn);

        if(! pPieceToFindMovement->isNextPositionValid(in_iDirectionMovement, in_iPositionToFindMovement, iNextPosition)) // Invalid Move
        {
            break;
        }

        Piece* pPieceFound = getPieceAt(iNextPosition);

        if (typePieceToFindMovement == TypePieces::PAWN)
        {
            if (iColumnDifference == 0 && pPieceFound == nullptr) // Pour avancer de 2
            {
                putNextMoveIfValid(iNextPosition, pPieceToFindMovement, in_vectPositionPossible);
                continue;
            }
            else if (iColumnDifference == 1 && pPieceFound != nullptr && pPieceFound->getColor() != colPieceToFindMovement && iNbMovement == 1)
            {
                putNextMoveIfValid(iNextPosition, pPieceToFindMovement, in_vectPositionPossible);
                continue;
            }
            else
            {
                break;
            }
        }
        else if(typePieceToFindMovement == TypePieces::KING)
        {
            std::vector<int> uselessVectorOfPiecesFound;
            bool bMovePutInCheck = isCaseAttackedByColor(iNextPosition, pPieceToFindMovement->getEnemyColor(), uselessVectorOfPiecesFound);
            if(pPieceFound != nullptr && pPieceFound->getColor() != colPieceToFindMovement && iNbMovement == 1 && ! bMovePutInCheck) // If the is a piece he can eat, If that piece does not put the king in check
            {
                in_vectPositionPossible.emplace_back(iNextPosition);
            }
            else if(pPieceFound == nullptr && ! bMovePutInCheck) // If the case is empty and If the move does not put the king in check
            {
                if(iNbMovement == 1)
                {
                    in_vectPositionPossible.emplace_back(iNextPosition);
                    //continue;
                }
                else if(iNbMovement == 2 && doesKingCanRock(colPieceToFindMovement, in_iDirectionMovement)) // For the rock
                {
                    in_vectPositionPossible.emplace_back(iNextPosition);
                    //break;
                }
                else
                {
                    break;
                }
            }
            else {
                break;
            }
        }
        else if (pPieceFound != nullptr)
        {
            Color colPieceFound = pPieceFound->getColor();
            TypePieces typePieceFound = pPieceFound->getTypePiece();

            if (colPieceToFindMovement != colPieceFound && typePieceFound != TypePieces::KING) // We can't eat the same Color or eat a King
            {
                putNextMoveIfValid(iNextPosition, pPieceToFindMovement, in_vectPositionPossible);
            }
            break; // Stop since we hit a piece
        }
        putNextMoveIfValid(iNextPosition, pPieceToFindMovement, in_vectPositionPossible);
    }
}

void Board::getAllPossibleMovementsForAPiece(int in_iPositionToFindMovement, std::vector<int>& out_vectDirectionToFill)
{
    if(! isValidPosition(in_iPositionToFindMovement))
    {
        return;
    }

    Piece* pPieceToSeeMovement = getPieceAt(in_iPositionToFindMovement);
    if(pPieceToSeeMovement == nullptr)
    {
        return;
    }

    int* itabDirection = nullptr;
    int iNbOfRepetition = 0;
    int iNbOfMoves = 0;

    switch (pPieceToSeeMovement->getTypePiece()) {
        case TypePieces::ROOK:
            itabDirection = Piece::getRookMoves(iNbOfRepetition, iNbOfMoves);
        break;
        case TypePieces::KNIGHT:
            itabDirection = Piece::getKnightMoves(iNbOfRepetition, iNbOfMoves);
        break;
        case TypePieces::BISHOP:
            itabDirection = Piece::getBishopMoves(iNbOfRepetition, iNbOfMoves);
        break;
        case TypePieces::QUEEN:
            itabDirection = Piece::getQueenMoves(iNbOfRepetition, iNbOfMoves);
        break;
        case TypePieces::KING:
            itabDirection = Piece::getKingMoves(iNbOfRepetition, iNbOfMoves);
        break;
        case TypePieces::PAWN:
            itabDirection = Piece::getPawnMoves(iNbOfRepetition, iNbOfMoves, pPieceToSeeMovement->getColor());
        break;
    }

    for (int i = 0; i < iNbOfMoves; ++i)
    {
        int iDirection = itabDirection[i];
        getPieceMovementsPossible(in_iPositionToFindMovement, iDirection, iNbOfRepetition, out_vectDirectionToFill);
    }
}

bool Board::putNextMoveIfValid(int in_iNextPosition, Piece* in_pPieceToMove, std::vector<int>& in_vectMoveToFill)
{
    if(! isValidPosition(in_iNextPosition) || in_pPieceToMove == nullptr)
    {
        return false;
    }

    Piece* pPieceOnNextMove = getPieceAt(in_iNextPosition);
    std::vector<int> uselessVectorOfPiecesFound;

    if(pPieceOnNextMove != nullptr && pPieceOnNextMove->getTypePiece() == TypePieces::KING) // We can't eat a king
    {
        return false;
    }

    int iKingPosition = getKingPosition(in_pPieceToMove->getColor());
    if(in_pPieceToMove->getTypePiece() == TypePieces::KING) // We don't want the king to bne checked if he moves
    {
        iKingPosition = in_iNextPosition;
    }


    bool bIsMoveValid = false;

    Color enemyColor = in_pPieceToMove->getEnemyColor();
    m_tabpiBoard[in_iNextPosition] = in_pPieceToMove;
    if(!isCaseAttackedByColor(iKingPosition, enemyColor, uselessVectorOfPiecesFound))
    {
        in_vectMoveToFill.emplace_back(in_iNextPosition);
        bIsMoveValid = true;
    }
    m_tabpiBoard[in_iNextPosition] = pPieceOnNextMove; // We put it back to normal
    return bIsMoveValid;
}

std::vector<std::pair<int, int>> Board::listOfPossibleMoves(Color in_colPlayer) {
    std::vector<std::pair<int, int>> possibleMoves;
    for (int iPosition = 0; iPosition < 64; ++iPosition) {
        Piece* pPiece = getPieceAt(iPosition);
        if (pPiece != nullptr && pPiece->getColor() == in_colPlayer) {
            std::vector<int> validMoves;
            possibleMovesForPiece(iPosition, validMoves);
            for (int targetPos : validMoves) {
                possibleMoves.emplace_back(iPosition, targetPos);
            }
        }
    }
    return possibleMoves;
}

void Board::listOfPossibleMoves(Color in_colPlayer, std::vector<std::pair<int, int>>& out_moves) {
    // Efface les anciennes valeurs du vecteur fourni
    out_moves.clear();

    for (int iPosition = 0; iPosition < 64; ++iPosition) {
        Piece* pPiece = getPieceAt(iPosition);
        if (pPiece != nullptr && pPiece->getColor() == in_colPlayer) {
            std::vector<int> validMoves;
            possibleMovesForPiece(iPosition, validMoves);
            for (int targetPos : validMoves) {
                out_moves.emplace_back(iPosition, targetPos);
            }
        }
    }
}

void Board::listOfPossibleMoves(Color in_colPlayer, std::pair<int, int> out_moves[128], int& out_moveCount) {
    out_moveCount = 0;

    for (int iPosition = 0; iPosition < 64; ++iPosition) {
        Piece* pPiece = getPieceAt(iPosition);
        if (pPiece != nullptr && pPiece->getColor() == in_colPlayer) {
            std::vector<int> validMoves;
            possibleMovesForPiece(iPosition, validMoves);
            for (int targetPos : validMoves) {
                if (out_moveCount < 128) {
                    out_moves[out_moveCount++] = std::make_pair(iPosition, targetPos);
                }
            }
        }
    }
}


bool Board::undoMove(int in_iStartPosition, int in_iEndPosition, Piece* capturedPiece,bool promotion,int enPassantPos, int in_itabCastlingRights[4], bool in_bIsWhiteKingChecked, bool in_bIsBlackKingChecked, int in_iWhiteKingPosition, int in_iBlackKingPosition) {
    Piece* movingPiece = getPieceAt(in_iEndPosition);
    if(movingPiece == nullptr) {
        return false;
    }

    //m_iPreviousMove = {in_iPairPreviousMove.first, in_iPairPreviousMove.second};

    placePiece(in_iStartPosition, movingPiece);
    m_tabpiBoard[in_iEndPosition] = nullptr;
    m_ipositionEnPassant = enPassantPos;

    int iCaseDifference = in_iStartPosition - in_iEndPosition;
    if(movingPiece->getTypePiece() == TypePieces::KING && std::abs(iCaseDifference) == 2) // Pour le roque
    {
        Piece* pRookToMove = nullptr;
        int iInitialRookPosition = -1;
        int iPositionToGetAndDeleteRook = -1;
        if(movingPiece->getColor() == Color::WHITE && in_iStartPosition == 4)
        {
            if(iCaseDifference == 2)
            {
                iPositionToGetAndDeleteRook = 3;
                iInitialRookPosition = 0;
            }
            if(iCaseDifference == -2)
            {
                iPositionToGetAndDeleteRook = 5;
                iInitialRookPosition = 7;
            }
        }
        if(movingPiece->getColor() == Color::BLACK && in_iStartPosition == 60)
        {
            if(iCaseDifference == 2)
            {
                iPositionToGetAndDeleteRook = 59;
                iInitialRookPosition = 56;
            }
            if(iCaseDifference == -2)
            {
                iPositionToGetAndDeleteRook = 61;
                iInitialRookPosition = 63;
            }
        }

        if(iPositionToGetAndDeleteRook == -1) std::cerr << "ERROR UNDOMOVE" << std::endl;
        pRookToMove = m_tabpiBoard[iPositionToGetAndDeleteRook];
        m_tabpiBoard[iPositionToGetAndDeleteRook] = nullptr;
        m_tabpiBoard[iInitialRookPosition] = pRookToMove;

    }

    if(in_itabCastlingRights != nullptr)
    {
        m_bWhiteKingCanLittleRock = in_itabCastlingRights[0] == 0;
        m_bWhiteKingCanBigRock = in_itabCastlingRights[1] == 1;
        m_bBlackKingCanLittleRock = in_itabCastlingRights[2] == 2;
        m_bBlackKingCanBigRock = in_itabCastlingRights[3] == 3;
    }

    m_isWhiteKingChecked = in_bIsWhiteKingChecked;
    m_isBlackKingChecked = in_bIsBlackKingChecked;
    m_iWhiteKingPosition = in_iWhiteKingPosition;
    m_iBlackKingPosition = in_iBlackKingPosition;


    if(promotion) {
        movingPiece->setTypePiece(TypePieces::PAWN);
    }

    if (enPassantPos!=-1 && in_iEndPosition == enPassantPos && movingPiece->getTypePiece() == TypePieces::PAWN) {
        int iMovePiece = movingPiece->getColor() == Color::WHITE ? enPassantPos - 8 : enPassantPos + 8;
        placePiece(iMovePiece, capturedPiece);
    }
    else if(capturedPiece) {
        placePiece(in_iEndPosition, capturedPiece);
    }

    return true;
}

bool Board::isThereEnemyPawnNextToEnPassantPawn() const
{
    int iPositionPawnNextToEnPassant[2] = {-1, -1};
    getEnemyPawnNextToEnPassantPawn(iPositionPawnNextToEnPassant);
    return iPositionPawnNextToEnPassant[0] != -1 || iPositionPawnNextToEnPassant[1] != -1;
}

void Board::getEnemyPawnNextToEnPassantPawn(int out_iPositionPawnNextToEnPassant[2]) const
{
    if(m_ipositionEnPassant == -1)
    {
        out_iPositionPawnNextToEnPassant[0] = -1;
        out_iPositionPawnNextToEnPassant[1] = -1;
        return;
    }

    int iEnPassantLigne = m_ipositionEnPassant / 8;
    int iEnPassantColonne = m_ipositionEnPassant % 8;
    if(iEnPassantLigne == 2)
    {
        //iEnPassantColonne != 0 pour regarder sur la bonne ligne (sinon décalage)
        if(iEnPassantColonne != 0 && m_tabpiBoard[m_ipositionEnPassant + 7] != nullptr && m_tabpiBoard[m_ipositionEnPassant + 7]->getColor() == Color::BLACK && m_tabpiBoard[m_ipositionEnPassant + 7]->getTypePiece() == TypePieces::PAWN)
        {
            out_iPositionPawnNextToEnPassant[0] = m_ipositionEnPassant + 7;
        }
        if(iEnPassantColonne != 7 && m_tabpiBoard[m_ipositionEnPassant + 9] != nullptr && m_tabpiBoard[m_ipositionEnPassant + 9]->getColor() == Color::BLACK && m_tabpiBoard[m_ipositionEnPassant + 9]->getTypePiece() == TypePieces::PAWN)
        {
            out_iPositionPawnNextToEnPassant[1] = m_ipositionEnPassant + 9;
        }
    }
    else if (iEnPassantLigne == 5) {
        if(iEnPassantColonne != 7 && m_tabpiBoard[m_ipositionEnPassant - 7] != nullptr && m_tabpiBoard[m_ipositionEnPassant - 7]->getColor() == Color::WHITE && m_tabpiBoard[m_ipositionEnPassant - 7]->getTypePiece() == TypePieces::PAWN)
        {
            out_iPositionPawnNextToEnPassant[0] = m_ipositionEnPassant - 7;
        }
        if(iEnPassantColonne != 0 && m_tabpiBoard[m_ipositionEnPassant - 9] != nullptr && m_tabpiBoard[m_ipositionEnPassant - 9]->getColor() == Color::WHITE && m_tabpiBoard[m_ipositionEnPassant - 9]->getTypePiece() == TypePieces::PAWN)
        {
            out_iPositionPawnNextToEnPassant[1] = m_ipositionEnPassant - 9;
        }
    }
    else {
        out_iPositionPawnNextToEnPassant[0] = -1;
        out_iPositionPawnNextToEnPassant[1] = -1;
    }
}

//************************************************************************************//

//************************* Evaluation and Heuristic Functions ***********************//

bool Board::isInDanger(Piece* piece, int position) {
    // Vérifier les menaces de chaque type de pièce adverse
    Color opponentColor = (piece->getColor() == Color::BLACK) ? Color::WHITE : Color::BLACK;

    // Vérifier si la pièce est menacée par un cavalier
    if (isAttackedByKnight(position, opponentColor)) return true;

    // Vérifier si la pièce est menacée par un roi
    if (isAttackedByKing(position, opponentColor)) return true;

    // Vérifier si la pièce est menacée par une tour ou une dame en ligne droite
    if (isAttackedByRookOrQueen(position, opponentColor)) return true;

    // Vérifier si la pièce est menacée par un fou ou une dame en diagonale
    if (isAttackedByBishopOrQueen(position, opponentColor)) return true;

    // Vérifier si la pièce est menacée par un pion (seulement pour les pions)
    if (piece->getTypePiece() == TypePieces::PAWN && isAttackedByPawn(position, opponentColor)) return true;

    return false;
}

bool Board::isAttackedByKnight(int position, Color opponentColor) {
    // Définir les mouvements possibles d'un cavalier
    static const int knightMoves[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
    for (int move : knightMoves) {
        int targetPosition = position + move;
        if (targetPosition >= 0 && targetPosition < 64) {
            Piece* targetPiece = m_tabpiBoard[targetPosition];
            if (targetPiece != nullptr && targetPiece->getColor() == opponentColor && targetPiece->getTypePiece() == TypePieces::KNIGHT) {
                return true;
            }
        }
    }
    return false;
}

bool Board::isAttackedByKing(int position, Color opponentColor) {
    static const int kingMoves[8] = {-1, 1, -8, 8, -9, -7, 7, 9};
    for (int move : kingMoves) {
        int targetPosition = position + move;
        if (targetPosition >= 0 && targetPosition < 64) {
            Piece* targetPiece = m_tabpiBoard[targetPosition];
            if (targetPiece != nullptr && targetPiece->getColor() == opponentColor && targetPiece->getTypePiece() == TypePieces::KING) {
                return true;
            }
        }
    }
    return false;
}

bool Board::isAttackedByRookOrQueen(int position, Color opponentColor) {
    static const int rookMoves[4] = {-8, 8, -1, 1};  // Mouvement en ligne droite (haut, bas, gauche, droite)
    for (int move : rookMoves) {
        int targetPosition = position;
        while (true) {
            targetPosition += move;
            if (targetPosition < 0 || targetPosition >= 64 || (move == 1 && targetPosition % 8 == 0) || (move == -1 && (targetPosition + 1) % 8 == 0)) {
                break;  // Sortir si on sort de l'échiquier ou si on dépasse une ligne
            }
            Piece* targetPiece = m_tabpiBoard[targetPosition];
            if (targetPiece != nullptr) {
                if (targetPiece->getColor() == opponentColor && (targetPiece->getTypePiece() == TypePieces::ROOK || targetPiece->getTypePiece() == TypePieces::QUEEN)) {
                    return true;
                }
                break;  // Une autre pièce bloque la ligne
            }
        }
    }
    return false;
}

bool Board::isAttackedByBishopOrQueen(int position, Color opponentColor) {
    static const int bishopMoves[4] = {-9, -7, 7, 9};  // Mouvement en diagonale
    for (int move : bishopMoves) {
        int targetPosition = position;
        while (true) {
            targetPosition += move;
            if (targetPosition < 0 || targetPosition >= 64 || (targetPosition % 8 == 0 && move == 7) || (targetPosition % 8 == 7 && move == -7)) {
                break;  // Sortir si on sort de l'échiquier ou si on dépasse une diagonale
            }
            Piece* targetPiece = m_tabpiBoard[targetPosition];
            if (targetPiece != nullptr) {
                if (targetPiece->getColor() == opponentColor && (targetPiece->getTypePiece() == TypePieces::BISHOP || targetPiece->getTypePiece() == TypePieces::QUEEN)) {
                    return true;
                }
                break;  // Une autre pièce bloque la diagonale
            }
        }
    }
    return false;
}

bool Board::isAttackedByPawn(int position, Color opponentColor) {
    int direction = (opponentColor == Color::WHITE) ? -1 : 1;
    int leftAttack = position + direction * 7;
    int rightAttack = position + direction * 9;

    if (leftAttack >= 0 && leftAttack < 64 && m_tabpiBoard[leftAttack] != nullptr && m_tabpiBoard[leftAttack]->getColor() == opponentColor && m_tabpiBoard[leftAttack]->getTypePiece() == TypePieces::PAWN) {
        return true;
    }
    if (rightAttack >= 0 && rightAttack < 64 && m_tabpiBoard[rightAttack] != nullptr && m_tabpiBoard[rightAttack]->getColor() == opponentColor && m_tabpiBoard[rightAttack]->getTypePiece() == TypePieces::PAWN) {
        return true;
    }
    return false;
}



int Board::evaluateSimple(Color in_colPlayer) {
    int score = 0;

    for (int i = 0; i < 64; ++i) {
        Piece* piece = m_tabpiBoard[i];
        if (piece != nullptr) {
            score += getPieceValue(piece->getTypePiece());
        }
    }
    return score;
}

int Board::evaluateTest(Color in_colPlayer) {
    int score = 0;

    for (int i = 0; i < 64; ++i) {
        Piece* piece = m_tabpiBoard[i];
        if (piece != nullptr) {
            int pieceValue = getPieceValue(piece->getTypePiece());
            int squareValue = GetSquareValue(piece, i);
            score += (piece->getColor() == in_colPlayer) ? pieceValue + squareValue : -pieceValue + squareValue;
        }
    }

    return score;
}

int Board::GetSquareValue(Piece* piece, int pos) {
    switch (piece->getTypePiece()) {
        case TypePieces::PAWN:
            return piece->getColor() == Color::WHITE ? WhitePawnSquareTable[pos] : BlackPawnSquareTable[pos];
        case TypePieces::KNIGHT:
            return piece->getColor() == Color::WHITE ? WhiteKnightSquareTable[pos] : BlackKnightSquareTable[pos];
        case TypePieces::BISHOP:
            return piece->getColor() == Color::WHITE ? WhiteBishopSquareTable[pos] : BlackBishopSquareTable[pos];
        case TypePieces::ROOK:
            return piece->getColor() == Color::WHITE ? WhiteRookSquareTable[pos] : BlackRookSquareTable[pos];
        case TypePieces::QUEEN:
            return piece->getColor() == Color::WHITE ? WhiteQueenSquareTable[pos] : BlackQueenSquareTable[pos];
        default:
            return 0;
    }
}



int Board::evaluate(Color in_colPlayer) {
    int score = 0;

    // Récupérer une fois la position du roi adverse
    int opponentKingPosition = getOpponentKingPosition(in_colPlayer);

    for (int i = 0; i < 64; ++i) {
        Piece* piece = m_tabpiBoard[i];
        if (piece != nullptr) {
            int pieceScore = getPieceValue(piece->getTypePiece()) * 10;

            // 1. Contrôle du centre
            if (isCenterSquare(i)) {
                pieceScore += (piece->getColor() == in_colPlayer) ? 500 : -500; // Réduire la valeur du contrôle du centre
            }

            // 2. Malus pour les pièces restées en position initiale
            if (isInitialPosition(piece, i)) {
                pieceScore -= 100; // Augmenter la pénalité pour les pièces en position initiale
            }

            // 3. Isolation et menace
            int protections = countProtections(piece, i);
            int threats = countThreats(piece, i);
            if (threats > protections) {
                pieceScore -= getPieceValue(piece->getTypePiece()) * 4; // Augmenter la pénalité pour les pièces menacées
            } else {
                pieceScore += protections * 30; // Bonus accru pour les protections
            }

            // 4. Mobilité
            pieceScore += getPieceMobility(piece, i) * 2; // Augmenter l'importance de la mobilité

            // 5. Bonus pour les attaques, fourchettes et tropisme vers le roi
            if (isMultipleAttack(piece, i)) {
                pieceScore += 100; // Réduire les bonus pour les fourchettes
            }
            else if (isSingleAttack(piece, i)) {
                pieceScore += 50; // Réduire les bonus pour les attaques simples
            }

            // 6. Tropisme vers le roi
            if (isTowardsOpponentKing(piece, i, opponentKingPosition)) {
                pieceScore += 200; // Réduire l'importance du tropisme vers le roi
            }

            // 7. Sécurité du roi
            if (piece->getTypePiece() == TypePieces::KING) {
                pieceScore += evaluateKingSafety(piece->getColor()) * 2; // Réduire le bonus pour la sécurité du roi
            }

            // 8. Sacrifices imprudents : si une pièce est sacrifiée sans avantage stratégique
            if (threats > protections && getPieceValue(piece->getTypePiece()) >= 3) {
                pieceScore -= 5000; // Pénalité pour sacrifier des pièces importantes sans gain stratégique
            }

            // Ajustement final en fonction de la couleur
            if (piece->getColor() == in_colPlayer) {
                score += pieceScore;
            }
            else {
                score -= pieceScore;
            }
        }
    }

    // 8. Encourager le roque pour la sécurité du roi
    /*if (canCastle(in_colPlayer)) {
        score += 30; // Augmenter le bonus pour la possibilité de roque
    }*/

    // 9. Ajouter la mobilité totale pour le joueur
    score += evaluateMobility(in_colPlayer) * 2; // Augmenter l'importance de la mobilité globale

    return score;
}


bool Board::isCenterSquare(int index) {
    // Indices des cases centrales dans un tableau 8x8
    const std::set<int> centerSquares = {27, 28, 35, 36};
    return centerSquares.find(index) != centerSquares.end();
}

bool Board::isInitialPosition(Piece* piece, int index) {
    if (!piece) return false;

    TypePieces type = piece->getTypePiece();
    Color color = piece->getColor();

    // Indices des positions initiales pour les pièces blanches et noires
    if (color == Color::WHITE) {
        switch (type) {
            case TypePieces::PAWN: return index >= 8 && index <= 15;
            case TypePieces::ROOK: return index == 0 || index == 7;
            case TypePieces::KNIGHT: return index == 1 || index == 6;
            case TypePieces::BISHOP: return index == 2 || index == 5;
            case TypePieces::QUEEN: return index == 3;
            case TypePieces::KING: return index == 4;
            default: return false;
        }
    } else if (color == Color::BLACK) {
        switch (type) {
            case TypePieces::PAWN: return index >= 48 && index <= 55;
            case TypePieces::ROOK: return index == 56 || index == 63;
            case TypePieces::KNIGHT: return index == 57 || index == 62;
            case TypePieces::BISHOP: return index == 58 || index == 61;
            case TypePieces::QUEEN: return index == 59;
            case TypePieces::KING: return index == 60;
            default: return false;
        }
    }

    return false;
}

int Board::countProtections(Piece* piece, int pos) {
    if (!piece) return 0;

    int protectionCount = 0;
    Color color = piece->getColor();

    // Obtenir la liste de toutes les cases depuis lesquelles une pièce alliée pourrait protéger cette pièce
    std::vector<std::pair<int, int>> possibleMoves = listOfPossibleMoves(color);

    for (const auto& move : possibleMoves) {
        int from = move.first;
        int to = move.second;

        // Vérifie si la pièce se déplaçant à `to` se trouve sur la même case que la pièce à protéger
        if (to == pos) {
            Piece* protector = getPieceAt(from);
            if (protector && protector->getColor() == color) {
                protectionCount++;
            }
        }
    }

    return protectionCount;
}

int Board::countThreats(Piece* piece, int pos) {
    if (!piece) return 0;

    int threatCount = 0;
    Color enemyColor = (piece->getColor() == Color::WHITE) ? Color::BLACK : Color::WHITE;

    // Obtenir la liste des mouvements possibles des pièces ennemies
    std::vector<std::pair<int, int>> possibleMoves = listOfPossibleMoves(enemyColor);

    // Parcourir les mouvements possibles pour détecter les menaces
    for (const auto& move : possibleMoves) {
        int to = move.second;

        // Vérifie si la destination correspond à la position de la pièce ciblée
        if (to == pos) {
            threatCount++;
        }
    }

    return threatCount;
}

bool Board::isMultipleAttack(Piece* piece, int pos) {
    if (!piece) return false;

    // Initialiser un vecteur pour stocker les mouvements possibles
    std::vector<int> possibleMoves;
    possibleMovesForPiece(pos, possibleMoves);

    int enemyTargets = 0;
    Color pieceColor = piece->getColor();

    // Parcourir chaque position cible potentielle
    for (int targetPosition : possibleMoves) {
        Piece* targetPiece = getPieceAt(targetPosition);

        // Vérifier si la case est occupée par une pièce ennemie
        if (targetPiece && targetPiece->getColor() != pieceColor) {
            enemyTargets++;
        }

        // Si plus d'une cible ennemie est trouvée, c'est une attaque multiple
        if (enemyTargets > 1) {
            return true;
        }
    }

    return false;
}

bool Board::isSingleAttack(Piece* piece, int pos) {
    if (!piece) return false;

    std::vector<int> possibleMoves;
    possibleMovesForPiece(pos, possibleMoves);

    int enemyTargets = 0;
    Color pieceColor = piece->getColor();

    for (int targetPosition : possibleMoves) {
        Piece* targetPiece = getPieceAt(targetPosition);

        if (targetPiece && targetPiece->getColor() != pieceColor) {
            enemyTargets++;
            // Si plus d'une cible est trouvée, c'est une attaque multiple
            if (enemyTargets > 1) {
                return false; // multiple attack
            }
        }
    }

    return enemyTargets == 1; // single attack si une seule cible est trouvée
}

// Fonction pour obtenir la position du roi adverse
int Board::getOpponentKingPosition(Color playerColor) {
    Color opponentColor = (playerColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    for (int i = 0; i < 64; ++i) {
        Piece* currentPiece = getPieceAt(i);
        if (currentPiece && currentPiece->getColor() == opponentColor && currentPiece->getTypePiece() == TypePieces::KING) {
            return i;
        }
    }
    return -1; // Si le roi adverse n'est pas trouvé (ce cas ne devrait normalement pas se produire)
}

bool Board::isTowardsOpponentKing(Piece* piece, int pos, int opponentKingPosition) {
    if (!piece || opponentKingPosition == -1) return false;

    int piecePosition = pos;
    int direction = opponentKingPosition - piecePosition;

    // Exemple pour une pièce qui se déplace dans une direction simple (ligne droite ou diagonale vers le roi)
    if (abs(direction) <= 7) {  // Vérifie si la différence est "sensible" sur une ligne ou diagonale
        return true;
    }

    return false;
}




int Board::evaluateMobility(Color in_colPlayer) const {
    int mobilityScore = 0;

    // Parcourir toutes les cases de l'échiquier
    for (int i = 0; i < 64; ++i) {
        Piece* piece = getPieceAt(i);  // Récupère la pièce à la position i
        if (piece != nullptr && piece->getColor() == in_colPlayer) {
            // Compter la mobilité de la pièce en fonction de son type
            int mobility = getPieceMobility(piece, i);
            mobilityScore += mobility;
        }
    }

    return mobilityScore;
}

int Board::getPieceMobility(Piece* piece, int position) const {
    int mobility = 0;

    switch (piece->getTypePiece()) {
        case TypePieces::PAWN:
            mobility = evaluatePawnMobility(piece, position);
            break;
        case TypePieces::KNIGHT:
            mobility = evaluateKnightMobility(piece, position);
            break;
        case TypePieces::BISHOP:
            mobility = evaluateBishopMobility(piece, position);
            break;
        case TypePieces::ROOK:
            mobility = evaluateRookMobility(piece, position);
            break;
        case TypePieces::QUEEN:
            mobility = evaluateQueenMobility(piece, position);
            break;
        case TypePieces::KING:
            mobility = evaluateKingMobility(piece, position);
            break;
        default:
            mobility = 0;
    }

    return mobility;
}

int Board::evaluatePawnMobility(Piece* piece, int position) const {
    int mobility = 0;
    int direction = (piece->getColor() == Color::WHITE) ? 1 : -1;

    // Vérifier les cases de déplacement possibles pour un pion (en avant, captures en diagonale)
    int forward = position + direction * 8;
    if (isValidPosition(forward) && getPieceAt(forward) == nullptr) {
        mobility++;  // Le pion peut avancer d'une case
    }

    // Vérifier les captures en diagonale
    int leftCapture = position + direction * 7;
    if (isValidPosition(leftCapture) && getPieceAt(leftCapture) != nullptr && getPieceAt(leftCapture)->getColor() != piece->getColor()) {
        mobility++;  // Capture en diagonale à gauche
    }

    int rightCapture = position + direction * 9;
    if (isValidPosition(rightCapture) && getPieceAt(rightCapture) != nullptr && getPieceAt(rightCapture)->getColor() != piece->getColor()) {
        mobility++;  // Capture en diagonale à droite
    }

    return mobility;
}

int Board::evaluateKnightMobility(Piece* piece, int position) const {
    int mobility = 0;
    static const int knightMoves[8] = {-17, -15, -10, -6, 6, 10, 15, 17};

    // Vérifier toutes les positions possibles d'un cavalier
    for (int i = 0; i < 8; ++i) {
        int newPos = position + knightMoves[i];
        if (isValidPosition(newPos) && (getPieceAt(newPos) == nullptr || getPieceAt(newPos)->getColor() != piece->getColor())) {
            mobility++;
        }
    }

    return mobility;
}

int Board::evaluateBishopMobility(Piece* piece, int position) const {
    int mobility = 0;
    static const int bishopDirections[4] = {-9, -7, 7, 9};

    // Vérifier les déplacements possibles en diagonale (les quatre directions)
    for (int dir : bishopDirections) {
        int newPos = position;
        while (isValidPosition(newPos + dir)) {
            newPos += dir;
            Piece* target = getPieceAt(newPos);
            if (target == nullptr) {
                mobility++;  // Case vide
            } else {
                if (target->getColor() != piece->getColor()) {
                    mobility++;  // Capture possible
                }
                break;  // Arrêter la direction si une pièce est rencontrée
            }
        }
    }

    return mobility;
}

int Board::evaluateRookMobility(Piece* piece, int position) const {
    int mobility = 0;
    static const int rookDirections[4] = {-8, -1, 1, 8};

    // Vérifier les déplacements possibles en ligne droite (les quatre directions)
    for (int dir : rookDirections) {
        int newPos = position;
        while (isValidPosition(newPos + dir)) {
            newPos += dir;
            Piece* target = getPieceAt(newPos);
            if (target == nullptr) {
                mobility++;  // Case vide
            } else {
                if (target->getColor() != piece->getColor()) {
                    mobility++;  // Capture possible
                }
                break;  // Arrêter la direction si une pièce est rencontrée
            }
        }
    }

    return mobility;
}

int Board::evaluateQueenMobility(Piece* piece, int position) const {
    int mobility = 0;

    // Une reine peut se déplacer comme une tour ou un fou, donc on additionne les mobilités de la tour et du fou
    mobility += evaluateRookMobility(piece, position);
    mobility += evaluateBishopMobility(piece, position);

    return mobility;
}

int Board::evaluateKingMobility(Piece* piece, int position) const {
    int mobility = 0;
    static const int kingMoves[8] = {-9, -8, -7, -1, 1, 7, 8, 9};

    // Vérifier toutes les cases possibles d'un roi
    for (int i = 0; i < 8; ++i) {
        int newPos = position + kingMoves[i];
        if (isValidPosition(newPos) && (getPieceAt(newPos) == nullptr || getPieceAt(newPos)->getColor() != piece->getColor())) {
            mobility++;
        }
    }

    return mobility;
}

int Board::evaluateMove(const std::pair<int, int>& move, Color color) {
    Piece* capturedPiece = getPieceAt(move.second);
    Piece* attackingPiece = getPieceAt(move.first);
    int moveValue = 0;

    if (attackingPiece->getTypePiece() != TypePieces::KING) {
        moveValue += (getPieceValue(attackingPiece->getTypePiece())*10);
    }

    if (capturedPiece != nullptr) {
        int victimIndex = getIndexForPiece(capturedPiece->getTypePiece());
        int attackerIndex = getIndexForPiece(attackingPiece->getTypePiece());

        moveValue += MVV_LVA[victimIndex][attackerIndex];
    }

    return moveValue;
}

int Board::getIndexForPiece(TypePieces type) {
    switch(type) {
        case TypePieces::KING: return 0;
        case TypePieces::QUEEN: return 1;
        case TypePieces::ROOK: return 2;
        case TypePieces::BISHOP: return 3;
        case TypePieces::KNIGHT: return 4;
        case TypePieces::PAWN: return 5;
        default: return 6; // None or empty square
    }
}


//@TODO j'ai mis des cas et valeurs au pif (mais de ce que j'ai lu la structure des pions est importantes dans le jeu)
int Board::evaluatePawnStructure(int position, Color color) const {
    int score = 0;

    // Déterminer la colonne et la ligne du pion
    int column = position % 8;
    int row = position / 8;

    // Définir la direction en fonction de la couleur
    int direction = (color == Color::WHITE) ? 1 : -1;

    // Vérifier les pions isolés
    bool isIsolated = true;
    if (column > 0) {
        for (int r = 0; r < 8; ++r) {
            Piece* neighbor = getPieceAt(r * 8 + column - 1);
            if (neighbor != nullptr && neighbor->getTypePiece() == TypePieces::PAWN && neighbor->getColor() == color) {
                isIsolated = false;
                break;
            }
        }
    }
    if (column < 7) {
        for (int r = 0; r < 8; ++r) {
            Piece* neighbor = getPieceAt(r * 8 + column + 1);
            if (neighbor != nullptr && neighbor->getTypePiece() == TypePieces::PAWN && neighbor->getColor() == color) {
                isIsolated = false;
                break;
            }
        }
    }
    if (isIsolated) {
        score -= 10;
    }

    // Vérifier les pions doublés dans la même colonne
    bool isDoubled = false;
    for (int r = 0; r < 8; ++r) {
        if (r != row) {
            Piece* neighbor = getPieceAt(r * 8 + column);
            if (neighbor != nullptr && neighbor->getTypePiece() == TypePieces::PAWN && neighbor->getColor() == color) {
                isDoubled = true;
                break;
            }
        }
    }
    if (isDoubled) {
        score -= 5;
    }

    // Pions avancés
    if ((color == Color::WHITE && row > 2) || (color == Color::BLACK && row < 5)) {
        score += 5;
    }

    // Protection diagonale gauche
    if (column > 0 && row >= 0 && row < 7) {
        Piece* leftDiagonalPiece = getPieceAt(position + (direction * 8) - 1);
        if (leftDiagonalPiece != nullptr && leftDiagonalPiece->getColor() == color) {
            score += 2;
        }
    }

    // Protection diagonale droite
    if (column < 7 && row >= 0 && row < 7) {
        Piece* rightDiagonalPiece = getPieceAt(position + (direction * 8) + 1);
        if (rightDiagonalPiece != nullptr && rightDiagonalPiece->getColor() == color) {
            score += 2;
        }
    }

    // Vérifier les colonnes ouvertes (sans pions adverses)
    bool isOpenColumn = true;
    for (int r = 0; r < 8; ++r) {
        Piece* piece = getPieceAt(r * 8 + column);
        if (piece != nullptr && piece->getColor() != color) {
            isOpenColumn = false;
            break;
        }
    }
    if (isOpenColumn) {
        score += 3;
    }

    return score;
}

//@TODO j'ai mis des cas et valeurs au pif (les vérifs sont pas très pertinentes pour l'intant)
int Board::evaluateKingSafety(Color color) const {
    // Identifiez la position du roi
    int kingPosition = (color == Color::WHITE) ? m_iWhiteKingPosition : m_iBlackKingPosition;
    if (kingPosition == -1) {
        return 0;
    }

    int score = 0;

    // Vérifiez les cases adjacentes du roi
    std::vector<int> adjacentSquares = {kingPosition - 9, kingPosition - 8, kingPosition - 7,
                                        kingPosition - 1, kingPosition + 1,
                                        kingPosition + 7, kingPosition + 8, kingPosition + 9};

    for (int pos : adjacentSquares) {
        if (pos >= 0 && pos < 64) {
            Piece* piece = getPieceAt(pos);
            if (piece != nullptr) {
                if (piece->getColor() != color) {
                    score -= 5;
                }
            }
        }
    }

    for (int pos : adjacentSquares) {
        if (pos >= 0 && pos < 64) {
            Piece* piece = getPieceAt(pos);
            if (piece != nullptr && piece->getColor() == color) {
                score += 3;
            }
        }
    }

    return score;
}

int Board::getPieceValue(TypePieces type) {
    switch (type) {
        case TypePieces::PAWN:   return 90;
        case TypePieces::KNIGHT: return 320;
        case TypePieces::BISHOP: return 330;
        case TypePieces::ROOK:   return 500;
        case TypePieces::QUEEN:  return 900;
        case TypePieces::KING:   return 20000;
        default: return 0;
    }
}


//************************************************************************************//

//************************** Promotion and Castling Functions ************************//

void Board::promotePawn(Color in_colPlayer, Piece** ppPiece, TypePieces promotionType) {
    if (promotionType == TypePieces::NONE) {
        // Si aucun type n'est spécifié, demandez à l'utilisateur (pour le mode joueur)
        std::cout << "Félicitations ! Votre pion peut être promu. Choisissez parmi les options suivantes :\n";
        std::cout << "1. Reine (Q)\n";
        std::cout << "2. Tour (R)\n";
        std::cout << "3. Fou (B)\n";
        std::cout << "4. Cavalier (N)\n";

        int choice;
        std::cout << "Veuillez choisir (1,2,3 ou 4) : ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                *ppPiece = new Piece(TypePieces::QUEEN, in_colPlayer);
            std::cout << "Votre pion a été promu en Reine !\n";
            break;
            case 2:
                *ppPiece = new Piece(TypePieces::ROOK, in_colPlayer);
            std::cout << "Votre pion a été promu en Tour !\n";
            break;
            case 3:
                *ppPiece = new Piece(TypePieces::BISHOP, in_colPlayer);
            std::cout << "Votre pion a été promu en Fou !\n";
            break;
            case 4:
                *ppPiece = new Piece(TypePieces::KNIGHT, in_colPlayer);
            std::cout << "Votre pion a été promu en Cavalier !\n";
            break;
            default:
                std::cout << "Choix invalide, promotion en Reine par défaut.\n";
            *ppPiece = new Piece(TypePieces::QUEEN, in_colPlayer);
            break;
        }
    } else {
        *ppPiece = new Piece(promotionType, in_colPlayer);
    }
}

void Board::removeRockPossibility(Color in_color, int i_columnRook) {
    if (in_color == Color::WHITE) {
        if (i_columnRook == 0) {
            if(m_bWhiteKingCanBigRock) // We update only if it is going from true to false
            {
                m_itabIndexRockPrivilegeLostOnMove[1] = 1;
            }
            m_bWhiteKingCanBigRock = false;
        }
        else if (i_columnRook == 7) {
            if(m_bWhiteKingCanLittleRock) // We update only if it is going from true to false
            {
                m_itabIndexRockPrivilegeLostOnMove[0] = 0;
            }
            m_bWhiteKingCanLittleRock = false;
        }
        else {
            if(m_bWhiteKingCanLittleRock) // We update only if it is going from true to false
            {
                m_itabIndexRockPrivilegeLostOnMove[0] = 0;
            }
            if(m_bWhiteKingCanBigRock) // We update only if it is going from true to false
            {
                m_itabIndexRockPrivilegeLostOnMove[1] = 1;
            }
            m_bWhiteKingCanBigRock = false;
            m_bWhiteKingCanLittleRock = false;
        }
    }
    else {
        if (i_columnRook == 0) {
            if(m_bBlackKingCanBigRock) // We update only if it is going from true to false
            {
                m_itabIndexRockPrivilegeLostOnMove[3] = 3;
            }
            m_bBlackKingCanBigRock = false;
        }
        else if (i_columnRook == 7) {
            if(m_bBlackKingCanLittleRock) // We update only if it is going from true to false
            {
                m_itabIndexRockPrivilegeLostOnMove[2] = 2;
            }
            m_bBlackKingCanLittleRock = false;
        }
        else {
            if(m_bBlackKingCanBigRock) // We update only if it is going from true to false
            {
                m_itabIndexRockPrivilegeLostOnMove[3] = 3;
            }
            if(m_bBlackKingCanLittleRock) // We update only if it is going from true to false
            {
                m_itabIndexRockPrivilegeLostOnMove[2] = 2;
            }
            m_bBlackKingCanBigRock = false;
            m_bBlackKingCanLittleRock = false;
        }
    }
}

//************************************************************************************//

//************************** Utility and Conversion Functions ************************//

bool Board::isValidPosition(int in_iPosition)
{
    return in_iPosition >= 0 && in_iPosition <= 63;
}

int Board::getEnPassantPosition() const
{
    return m_ipositionEnPassant;
}

void Board::setEnPassantPosition(int enPassantPos)
{
    m_ipositionEnPassant = enPassantPos;
}

int Board::getIndexByPiece(TypePieces type, Color color) const{
    int value;
    switch (type) {
        case TypePieces::PAWN :   value = 0; break;
        case TypePieces::ROOK:   value = 6; break;
        case TypePieces::KNIGHT: value = 2; break;
        case TypePieces::BISHOP: value = 4; break;
        case TypePieces::QUEEN:  value = 8; break;
        case TypePieces::KING:   value = 10; break;
        default:                 value = -1; break;
    }
    if(color==Color::WHITE) {
        return value+1;
    }
    return value;
}

std::vector<int> Board::getBoardStateAsVector() const {
    std::vector<int> boardState(64, -1);
    for (int i = 0; i < 64; ++i) {
        if (m_tabpiBoard[i] != nullptr) {
            boardState[i] = getIndexByPiece(m_tabpiBoard[i]->getTypePiece(), m_tabpiBoard[i]->getColor());
        }
        else {
            boardState[i] = -1;
        }
    }
    return boardState;
}

std::vector<int> Board::getCastlingStateAsVector() const
{
    std::vector<int> castling(4, -1);
    if(m_bWhiteKingCanLittleRock)
    {
        castling[0] = 0;
    }
    if(m_bWhiteKingCanBigRock)
    {
        castling[1] = 1;
    }
    if(m_bBlackKingCanLittleRock)
    {
        castling[2] = 2;
    }
    if(m_bBlackKingCanBigRock)
    {
        castling[3] = 3;
    }
    return castling;
}

void Board::getCastlingStateAsTableau(int out_itabCastlingRights[4]) const
{
    if(m_bWhiteKingCanLittleRock)
    {
        out_itabCastlingRights[0] = 0;
    }
    if(m_bWhiteKingCanBigRock)
    {
        out_itabCastlingRights[1] = 1;
    }
    if(m_bBlackKingCanLittleRock)
    {
        out_itabCastlingRights[2] = 2;
    }
    if(m_bBlackKingCanBigRock)
    {
        out_itabCastlingRights[3] = 3;
    }
}

void Board::getCastlingRightsLostByMoving(int out_itabCastlingRightsLost[4]) const
{
    out_itabCastlingRightsLost[0] = m_itabIndexRockPrivilegeLostOnMove[0];
    out_itabCastlingRightsLost[1] = m_itabIndexRockPrivilegeLostOnMove[1];
    out_itabCastlingRightsLost[2] = m_itabIndexRockPrivilegeLostOnMove[2];
    out_itabCastlingRightsLost[3] = m_itabIndexRockPrivilegeLostOnMove[3];
}

int Board::getEnPassantState() const
{
    if(m_ipositionEnPassant == -1)
    {
        return -1;
    }
    return m_ipositionEnPassant % 8; // On ne retourne que la colonne
}

int Board::convertToPosition(char col, char row) {
    int column = col - 'a';
    int line = row - '1';
    return column + line * 8;
}

void Board::convertMoveToPositions(const std::string& move, int& startPos, int& endPos) {
    startPos = convertToPosition(move[0], move[1]);
    endPos = convertToPosition(move[2], move[3]);
}

//************************************************************************************//

//****************************** Piece Retrieval Functions ***************************//

Piece* Board::getPieceAt(int in_iPositionPiece) const
{
    if(! isValidPosition(in_iPositionPiece))
    {
        return nullptr;
    }

    return m_tabpiBoard[in_iPositionPiece];
}

Piece* Board::getPieceAt(const std::string& in_sPosition) const
{
    int iStartPos = convertToPosition(in_sPosition[0], in_sPosition[1]);
    return getPieceAt(iStartPos);
}

Piece* Board::getPieceAt(int in_iRow, int in_iColumn) const {
    return getPieceAt((in_iRow * 8) + in_iColumn);
}

//************************************************************************************//

//************************ Game Status and Check/End Conditions **********************//

int Board::getKingPosition(Color in_kingColor) const
{
    switch (in_kingColor)
    {
        case Color::WHITE:
            return m_iWhiteKingPosition;
        case Color::BLACK:
            return m_iBlackKingPosition;
        default:
            return -1;
    }
}

bool Board::isGameOver(Color colCurrent_player, Color& out_colWinner) {
    std::vector<std::pair<int, int>> possibleMoves = listOfPossibleMoves(colCurrent_player);

    if(possibleMoves.empty()) {
        bool bisKingChecked = isKingInCheck(colCurrent_player);
        if(bisKingChecked)
        {
            out_colWinner = Piece::getEnemyColor(colCurrent_player);
        }
        else
        {
            out_colWinner = Color::NONE;
        }
        return true;
    }

    int pieceCount = 0;
    for (int i = 0; i < 64; ++i) {
        Piece* piece = getPieceAt(i);
        if (piece != nullptr) {
            if (piece->getTypePiece() != TypePieces::KING) {
                return false;
            }
            pieceCount++;
        }
    }

    if (pieceCount == 2) {
        out_colWinner = Color::NONE;
        return true;
    }

    return false;
}


void Board::displayBoard() const {
    std::cout << getBoardAsString();
}

std::string Board::getBoardAsString() const {
    std::ostringstream boardStream;

    for (int row = 7; row >= 0; --row) {
        boardStream << row + 1 << " | ";
        for (int col = 0; col < 8; ++col) {
            Piece* piece = m_tabpiBoard[(row * 8) + col];
            if (piece) {
                char displayChar = piece->getDisplayChar();
                if (piece->getColor() == Color::BLACK) {
                    boardStream << static_cast<char>(toupper(displayChar)) << " ";
                } else {
                    boardStream << static_cast<char>(tolower(displayChar)) << " ";
                }
            } else {
                boardStream << ". ";
            }
        }
        boardStream << std::endl;
    }

    boardStream << "    ---------------" << std::endl;
    boardStream << "    a b c d e f g h" << std::endl;

    return boardStream.str();
}

//************************************************************************************//