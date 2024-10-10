//
// Created by Peter on 02/10/2024.
//

#include "Board.h"

#include <cmath>
#include <iostream>
Board::Board(): m_enPassantPosition{-1, -1}
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
        m_tabpiBoard[iIndiceRow] = nullptr;
    }

    m_isWhiteKingChecked = false;
    m_isBlackKingChecked = false;
}

bool Board::isWhiteKingCheck() const
{
    return m_isWhiteKingChecked;
}

bool Board::isBlackKingCheck() const
{
    return m_isBlackKingChecked;
}

bool Board::isValidPosition(int in_iPosition)
{
    return in_iPosition >= 0 && in_iPosition <= 63;
}

bool Board::placePiece(int in_iPositionPiece, Piece* in_pPiece)
{
    if(! isValidPosition(in_iPositionPiece))
    {
        return false;
    }

    m_tabpiBoard[in_iPositionPiece] = in_pPiece;
    return true;
}

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

int Board::convertToPosition(char col, char row) {
    int column = col - 'a';
    int line = row - '1';
    return column + line * 8;
}

void Board::convertMoveToPositions(const std::string& move, int& startPos, int& endPos) {
    startPos = convertToPosition(move[0], move[1]);
    endPos = convertToPosition(move[2], move[3]);
}

bool Board::movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer)
{
    return movePiece((in_iStartRow * 8) + in_iStartCol, (in_iEndRow * 8) + in_iEndCol, in_colPlayer);
}

bool Board::movePiece(const std::string& move, Color in_colPlayer)
{
    int iStartPosition, iEndPosition;
    convertMoveToPositions(move, iStartPosition, iEndPosition);
    return movePiece(iStartPosition, iEndPosition, in_colPlayer);
}

bool Board::placePiece(const std::string& move, Piece* in_pPiece)
{
    int iStartPos = convertToPosition(move[0], move[1]);
    return placePiece(iStartPos, in_pPiece);
}

bool Board::isKingInCheck(Color in_kingColor) const
{
    if(in_kingColor == Color::WHITE)
    {
        return m_isWhiteKingChecked;
    }
    return m_isBlackKingChecked;
}



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

    for(int iIndiceValidPosition = 0; iIndiceValidPosition < itabValidPositions.size(); iIndiceValidPosition++)
    {
        if(itabValidPositions[iIndiceValidPosition] == in_iTargetPosition)
        {
            return true;
        }
    }

    return false;
}

bool Board::movePiece(int in_iStartPosition, int in_iEndPosition, Color in_colPlayer,Piece** piece)
{
    Piece* pPiece = getPieceAt(in_iStartPosition);
    if(pPiece == nullptr || pPiece->getColor() != in_colPlayer) //If the player try to move a piece of another color, return false
    {
        return false;
    }

    if (isMovementPossible(in_iStartPosition,in_iEndPosition))
    {

        bool bKingWentRightForRock = in_iEndPosition - in_iStartPosition == 2;
        bool bKingWentLeftForRock = in_iEndPosition - in_iStartPosition == -2;

        if(pPiece->getTypePiece() == TypePieces::KING) // If the king rocked (move of length 2, we move the rook)
        {
            if(in_colPlayer == Color::WHITE)
            {
                m_iWhiteKingPosition = in_iEndPosition;
            }
            else
            {
                m_iBlackKingPosition = in_iEndPosition;
            }

            Piece* pRookForRock = nullptr;
            if(bKingWentRightForRock)
            {
                int iLastPositionOnTheRow = in_iStartPosition + (7 - in_iStartPosition%8);
                pRookForRock = getPieceAt(iLastPositionOnTheRow);

                if(pRookForRock != nullptr)
                {
                    //pRookForRock->setAlreadyMoved(true);
                    bool bCouldPLacePiece = placePiece(in_iStartPosition + 1, pRookForRock);
                    if(! bCouldPLacePiece)
                    {
                        return false;
                    }
                    m_tabpiBoard[iLastPositionOnTheRow] = nullptr; // La dernière case de la ligne
                }
            }
            else if(bKingWentLeftForRock)
            {
                int iFirstPositionOnTheRow = in_iStartPosition - in_iStartPosition%8;
                pRookForRock = getPieceAt(in_iStartPosition);

                if(pRookForRock != nullptr)
                {
                    //pRookForRock->setAlreadyMoved(true);
                    bool bCouldPLacePiece = placePiece(in_iStartPosition - 1, pRookForRock);
                    if(! bCouldPLacePiece)
                    {
                        return false;
                    }
                    m_tabpiBoard[iFirstPositionOnTheRow] = nullptr; // La première case de la ligne
                }
            }
        }

        if(piece) {
            *piece = getPieceAt(in_iEndPosition);
        }
        placePiece(in_iEndPosition, pPiece);
        m_tabpiBoard[in_iStartPosition] = nullptr;

        return true;
    }

    return false;
}

void Board::findFirstPiecesOnEachRookMovements(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const
{
    if(! isValidPosition(in_iPosition))
    {
        return;
    }

    int currentCol = in_iPosition % 8;
    for (int i = 1; i < 8; ++i) {
        // Haut
        int upMove = in_iPosition + i * 8;
        if (!isValidPosition(upMove)) break;
        if (m_tabpiBoard[upMove] != nullptr) {
            in_vectPositionPieceFound.push_back(upMove);
            break;
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Bas
        int downMove = in_iPosition - i * 8;
        if (!isValidPosition(downMove)) break;
        if (m_tabpiBoard[downMove] != nullptr) {
            in_vectPositionPieceFound.push_back(downMove);
            break;
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Droite
        int rightMove = in_iPosition + i;
        if (currentCol+i < 7 && isValidPosition(rightMove)) {
            if (m_tabpiBoard[rightMove] != nullptr) {
                in_vectPositionPieceFound.push_back(rightMove);
                break;
            }
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Gauche
        int leftMove = in_iPosition - i;
        if (currentCol-i > 0 && isValidPosition(leftMove)) {
            if (m_tabpiBoard[leftMove] != nullptr) {
                in_vectPositionPieceFound.push_back(leftMove);
                break;
            }
        }
    }
}

void Board::putNextMoveIfValid(int in_iNextPosition, Piece* in_pPieceToMove, std::vector<int>& in_vectMoveToFill)
{
    if(! isValidPosition(in_iNextPosition) || in_pPieceToMove == nullptr)
    {
        return;
    }

    Piece* pPieceOnNextMove = getPieceAt(in_iNextPosition);
    std::vector<int> uselessVectorOfPiecesFound;

    //TODO Verify if we eat a king

    m_tabpiBoard[in_iNextPosition] = in_pPieceToMove;
    if(!isCaseAttackedByColor(in_iNextPosition, in_pPieceToMove->getEnemyColor(), uselessVectorOfPiecesFound))
    {
        in_vectMoveToFill.emplace_back(in_iNextPosition);
    }
    m_tabpiBoard[in_iNextPosition] = pPieceOnNextMove; // We put it back to normal
}

// DO not check if the cases are attacked
bool Board::doesKingCanRock(Color in_colKing, int in_iDirectionForRock) const
{
    int iKingPosition;
    switch(in_colKing)
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
    }

    int iPositionPieceFound = -1;
    Piece* pPieceFound = findFirstPieceOnDirection(iKingPosition, in_iDirectionForRock, 7, iPositionPieceFound);
    if(pPieceFound == nullptr)
    {
        return false;
    }

    if(pPieceFound->getTypePiece() == TypePieces::ROOK && iPositionPieceFound != -1)
    {
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

void Board::findFirstPiecesOnEachBishopMovements(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const
{
    if(! isValidPosition(in_iPosition))
    {
        return;
    }

    int currentCol = in_iPosition % 8;
    for (int i = 1; i < 8; ++i)
    {
        // Haut-Droite
        int upRightMove = in_iPosition + i * 9;
        if (currentCol+i<7 || !isValidPosition(upRightMove)) break;
        if (m_tabpiBoard[upRightMove] != nullptr)
        {
            in_vectPositionPieceFound.push_back(upRightMove);
            break;
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Haut-Gauche
        int upLeftMove = in_iPosition + i * 7; // 1 ligne en haut, 1 colonne à gauche
        if (currentCol-i>0 || !isValidPosition(upLeftMove)) break;
        if (m_tabpiBoard[upLeftMove] != nullptr)
        {
            in_vectPositionPieceFound.push_back(upLeftMove);
            break;
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Bas-Droite
        int downRightMove = in_iPosition - i * 7; // 1 ligne en bas, 1 colonne à droite
        if (currentCol+i<7 || !isValidPosition(downRightMove)) break;
        if (m_tabpiBoard[downRightMove] != nullptr)
        {
            in_vectPositionPieceFound.push_back(downRightMove);
            break;
        }
    }

    for (int i = 1; i < 8; ++i) {
        // Bas-Gauche
        int downLeftMove = in_iPosition - i * 9; // 1 ligne en bas, 1 colonne à gauche
        if (currentCol-i>0 || !isValidPosition(downLeftMove)) break;
        if (m_tabpiBoard[downLeftMove] != nullptr)
        {
            in_vectPositionPieceFound.push_back(downLeftMove);
            break;
        }
    }
}

void Board::findFirstPiecesOnEachKnightMovements(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const
{
    if(! isValidPosition(in_iPosition))
    {
        return;
    }
    int iUseless1, iUseless2;
    const int* knightMoves = Piece::getKnightMoves(iUseless1, iUseless2);
    int numMoves = 8;  // Définir le nombre de mouvements possibles pour le cavalier (à ajuster selon la taille du tableau)

    for (int i = 0; i < numMoves; ++i) {
        int knightPosition = in_iPosition + knightMoves[i];
        if (isValidPosition(knightPosition)) {
            if (m_tabpiBoard[knightPosition] == nullptr) {
                in_vectPositionPieceFound.push_back(knightPosition);
            }
        }
    }

}

bool Board::isCaseAttackedByAnyColor(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const
{
    if(! isValidPosition(in_iPosition))
    {
        return false;
    }

    findFirstPiecesOnEachKnightMovements(in_iPosition, in_vectPositionPieceFound);
    findFirstPiecesOnEachBishopMovements(in_iPosition, in_vectPositionPieceFound);
    findFirstPiecesOnEachRookMovements(in_iPosition, in_vectPositionPieceFound);

    if(in_vectPositionPieceFound.empty()) {
        return false;
    }
    return true;
}



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
    switch (typePieceToSeeMoves) {
        case TypePieces::PAWN: {
            int direction = (colPieceToSeeMoves == Color::WHITE) ? 1 : -1;
            int startRow = (colPieceToSeeMoves == Color::WHITE) ? 1 : 6;

            // Normal moves
            int forwardMove = in_iPositionToSeeMoves + direction * 8;
            if (isValidPosition(forwardMove) && m_tabpiBoard[forwardMove] == nullptr) {
                in_vectPossibleMoves.push_back(forwardMove);
            }

            int twoSteps = in_iPositionToSeeMoves + (direction * 16);
            if (in_iPositionToSeeMoves/8 == startRow) {
                if (isValidPosition(twoSteps) && m_tabpiBoard[forwardMove] == nullptr && m_tabpiBoard[twoSteps] == nullptr) {
                    in_vectPossibleMoves.push_back(twoSteps);
                }
            }

            // Capture moves
            int captureLeft = in_iPositionToSeeMoves + direction * 8 - 1;
            int captureRight = in_iPositionToSeeMoves + direction * 8 + 1;
            if (isValidPosition(captureLeft) && in_iPositionToSeeMoves % 8 != 0 && m_tabpiBoard[captureLeft] != nullptr && m_tabpiBoard[captureLeft]->getColor() != colPieceToSeeMoves) {
                in_vectPossibleMoves.push_back(captureLeft);
            }
            if (isValidPosition(captureRight) && in_iPositionToSeeMoves % 8 != 7 && m_tabpiBoard[captureRight] != nullptr && m_tabpiBoard[captureRight]->getColor() != colPieceToSeeMoves) {
                in_vectPossibleMoves.push_back(captureRight);
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
                    if (!pPieceToSeeMoves->isNextPositionValid(dir, in_iPositionToSeeMoves, nextPosition)) {
                        break;
                    }

                    if (m_tabpiBoard[nextPosition] == nullptr) {
                        in_vectPossibleMoves.push_back(nextPosition);
                    } else {
                        if (m_tabpiBoard[nextPosition]->getColor() != colPieceToSeeMoves) {
                            in_vectPossibleMoves.push_back(nextPosition);
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

                if (pPieceToSeeMoves->isNextPositionValid(i, in_iPositionToSeeMoves, nextPosition)) {
                    if (m_tabpiBoard[nextPosition] == nullptr || m_tabpiBoard[nextPosition]->getColor() != colPieceToSeeMoves) {
                        in_vectPossibleMoves.push_back(nextPosition);
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

                    if (!pPieceToSeeMoves->isNextPositionValid(dir, in_iPositionToSeeMoves, nextPosition) ){
                        break;
                    }

                    if (m_tabpiBoard[nextPosition] == nullptr) {
                        in_vectPossibleMoves.push_back(nextPosition);
                    } else {
                        if (m_tabpiBoard[nextPosition]->getColor() != colPieceToSeeMoves) {
                            in_vectPossibleMoves.push_back(nextPosition);
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

                    if (!pPieceToSeeMoves->isNextPositionValid(dir, in_iPositionToSeeMoves, nextPosition)) {
                        break;
                    }

                    if (m_tabpiBoard[nextPosition] == nullptr) {
                        in_vectPossibleMoves.push_back(nextPosition);
                    } else {
                        if (m_tabpiBoard[nextPosition]->getColor() != colPieceToSeeMoves) {
                            in_vectPossibleMoves.push_back(nextPosition);
                        }
                        break;
                    }
                }
            }
            break;
        }
        case TypePieces::KING: {
            const int* kingMoves = Piece::getKingMoves(iUseless, iUseless2);
            int numMoves = 8;

            for (int i = 0; i < numMoves; ++i) {
                int nextPosition = in_iPositionToSeeMoves + kingMoves[i];

                if (pPieceToSeeMoves->isNextPositionValid(i, in_iPositionToSeeMoves, nextPosition)) {
                    if (m_tabpiBoard[nextPosition] == nullptr || m_tabpiBoard[nextPosition]->getColor() != colPieceToSeeMoves) {
                        in_vectPossibleMoves.push_back(nextPosition);
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}


Piece* Board::findFirstPieceOnDirection(int in_iPosition,int in_iDirection, int in_iNbOfRepetition, int& in_iPositionPieceFound) const {
    Piece* pPieceToFindMovement = getPieceAt(in_iPosition);
    if(! isValidPosition(in_iPosition) || in_iNbOfRepetition <= 0 || pPieceToFindMovement == nullptr)
    {
        return nullptr;
    }

    in_iNbOfRepetition++;

    int iNextPosition = in_iPosition;




    for(int iNbMovement = 1; iNbMovement < in_iNbOfRepetition; iNbMovement++) {
        iNextPosition = iNextPosition + in_iDirection;

        if(! pPieceToFindMovement->isNextPositionValid(in_iDirection, in_iPosition, iNextPosition)) // Invalid Move
        {
            break;
        }

        Piece* pPieceFound = getPieceAt(iNextPosition);
        if(pPieceFound != nullptr)
        {
            in_iPositionPieceFound = in_iPosition + iNbMovement;
            return pPieceFound;
        }
    }

    in_iPositionPieceFound = 0;
    return nullptr;
}

std::vector<std::pair<int, std::vector<int>>> Board::listOfPossibleMovements(Color in_colColor)
{
    std::vector<std::pair<int, std::vector<int>>> vectPossibleMoves;

    for (int iPosition = 0; iPosition < 64; iPosition++)
    {
        Piece* pPieceToGetMovements = getPieceAt(iPosition);
        if (pPieceToGetMovements != nullptr)
        {
            std::vector<int> vectPieceMoves;
            getAllPossibleMovementsForAPiece(iPosition, vectPieceMoves);

            // Ajouter la position et les mouvements sous forme de paire
            vectPossibleMoves.emplace_back(iPosition, vectPieceMoves);
        }
    }

    return vectPossibleMoves;
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

int Board::evaluate(Color in_colPlayer) const {
    int score = 0;

    for(Piece* piece : m_tabpiBoard) {
        if(piece != nullptr) {
            int pieceScore = 0;
            switch(piece->getTypePiece()) {
                case TypePieces::PAWN:
                    pieceScore = 1;
                break;
                case TypePieces::KNIGHT:
                    pieceScore = 3;
                break;
                case TypePieces::BISHOP:
                    pieceScore = 3;
                break;
                case TypePieces::ROOK:
                    pieceScore = 5;
                break;
                case TypePieces::QUEEN:
                    pieceScore = 9;
                break;
                case TypePieces::KING:
                    pieceScore = INFINITY;
                break;
            }

            if(piece->getColor() == in_colPlayer) {
                score += pieceScore;
            } else {
                score -= pieceScore;
            }
        }
    }

    return score;
}

bool Board::isGameOver() const {
    return m_isGameOver;
}

bool Board::undoMove(int in_iStartPosition, int in_iEndPosition, Piece* capturedPiece) {
    Piece* movingPiece = getPieceAt(in_iEndPosition);
    if(movingPiece == nullptr) {
        return false;
    }

    placePiece(in_iStartPosition, movingPiece);
    m_tabpiBoard[in_iEndPosition] = nullptr;

    if(capturedPiece) {
        placePiece(in_iEndPosition, capturedPiece);
    }

    return true;
}

void Board::promotePawn(int in_iEndRow, int in_iEndCol, Color in_colPlayer, Piece** ppPiece) {
    std::cout << "Felicitations ! Votre pion peut être promu. Choisissez parmi les options suivantes :\n";
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
        std::cout << "Votre pion a ete promu en Reine !\n";
        break;
        case 2:
            *ppPiece = new Piece(TypePieces::ROOK, in_colPlayer);
        std::cout << "Votre pion a ete promu en Tour !\n";
        break;
        case 3:
            *ppPiece = new Piece(TypePieces::BISHOP, in_colPlayer);
        std::cout << "Votre pion a ete promu en Fou !\n";
        break;
        case 4:
            *ppPiece = new Piece(TypePieces::KNIGHT, in_colPlayer);
        std::cout << "Votre pion a ete promu en Cavalier !\n";
        break;
        default:
            std::cout << "Choix invalide, promotion en Reine par défaut.\n";
        *ppPiece = new Piece(TypePieces::QUEEN, in_colPlayer);
        break;
    }
}

void Board::displayBoard() const {
    for (int row = 7; row >= 0; --row) {
        std::cout << row + 1 << " | ";
        for (int col = 0; col < 8; ++col) {
            Piece* piece = m_tabpiBoard[(row * 8) + col];
            if (piece) {
                char displayChar = piece->getDisplayChar();
                if (piece->getColor() == Color::BLACK) {
                    std::cout << static_cast<char>(toupper(displayChar)) << " ";
                } else {
                    std::cout << static_cast<char>(tolower(displayChar)) << " ";
                }
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "    ---------------" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
}



/*
 *
 *-------------------------------------------------------------------------------- OLD CODE BELOW /!\ ------------------------------------------------------------------------------------------------------------------------------------------
 *
 */



bool Board::placePiece(int in_iRow, int in_iCol, Piece* in_pPiece)
{
    if(in_iRow < 0 || in_iRow >= 8 || in_iCol < 0 || in_iCol >= 8)
    {
        return false;
    }
    m_tabpiBoard[in_iRow*8+in_iCol] = in_pPiece;
    return true;
}

Piece* Board::getPieceAt(const Coordinate& coord) const
{
    if(coord.iRow < 0 || coord.iRow >= 8 || coord.iColumn < 0 || coord.iColumn >= 8)
    {
        return nullptr;
    }
    return m_tabtabpiBoard[coord.iRow][coord.iColumn];
}

Piece* Board::getPieceAt(int in_iRow, int in_iColumn) const {
    return getPieceAt((in_iRow * 8) + in_iColumn);
}


bool Board::doesPieceHaveGoodTypeOfAttack(Piece* in_pPieceToVerifyAttack, TypeOfPieceAttack in_typeOfAttack)
{
    if(in_pPieceToVerifyAttack == nullptr)
    {
        return false;
    }

    bool bGoodTypeOfAttackResearched = false;
    if(in_typeOfAttack == TypeOfPieceAttack::STRAIGHT)
    {
        bGoodTypeOfAttackResearched = in_pPieceToVerifyAttack->attackStraight();
    }
    else if(in_typeOfAttack == TypeOfPieceAttack::DIAGONAL)
    {
        bGoodTypeOfAttackResearched = in_pPieceToVerifyAttack->attackDiagonal();
    }
    else if(in_typeOfAttack == TypeOfPieceAttack::L)
    {
        bGoodTypeOfAttackResearched = in_pPieceToVerifyAttack->attackKnight();
    }
    return bGoodTypeOfAttackResearched;
}

bool Board::isMoveValid(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer)
{
    if(in_iStartRow >= 8 || in_iStartCol >= 8 || in_iStartRow < 0 || in_iStartCol < 0
        || in_iEndRow >= 8 || in_iEndCol >= 8 || in_iEndRow < 0 || in_iEndCol < 0)
    {
        return false;
    }

    Piece* pPieceToMove = getPieceAt(in_iStartRow, in_iStartCol);
    if(pPieceToMove == nullptr || pPieceToMove->getColor() != in_colPlayer) //If the player try to move a piece of another color, return false
    {
        return false;
    }

    if (pPieceToMove->getTypePiece() == TypePieces::PAWN) {
        int direction = (in_colPlayer == Color::WHITE) ? 1 : -1;
        if (m_enPassantPosition.getRow()+direction == in_iEndRow && m_enPassantPosition.getColumn() == in_iEndCol) {
            return true;
        }
    }

    Coordinate coordTargetPoint = Coordinate(in_iEndRow, in_iEndCol);
    // We get the piece to move
    std::vector<Coordinate> vectPossibleMoves/* = possibleMovesForPiece(Coordinate(in_iStartRow, in_iStartCol))*/;
    if(vectPossibleMoves.empty()) // Means that the piece could not move
    {
        return false;
    }
    else
    {
        return isCoordinateInVector(coordTargetPoint, vectPossibleMoves);
    }
}

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

bool Board::isCoordinateInVector(const Coordinate& coordTargetPoint, const std::vector<Coordinate>& vectPossibleMoves)
{
    for (const Coordinate& coord : vectPossibleMoves)
    {
        if (coord == coordTargetPoint)
        {
            return true; // Coordinate found in the vector
        }
    }
    return false; // Coordinate not found
}





