//
// Created by Peter on 02/10/2024.
//

#include "Board.h"
#include <iostream>
//TODO faire en sorte de stocker à chaque coup quelle pièce attaque quelle case pour plus d'opti
Board::Board(): m_enPassantPosition{-1, -1}
{
    for(int iIndiceRow = 0; iIndiceRow < 64; iIndiceRow++ )
    {
        m_tabpiBoard[iIndiceRow] = nullptr;
    }

    m_isWhiteKingChecked = false;
    m_isBlackKingChecked = false;
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


    // ------- Initialize the differents move possible for the pieces -------


    m_tabpiBoard[1]->movePiece(m_tabpiBoard, 1); // Left white knight
    m_tabpiBoard[6]->movePiece(m_tabpiBoard, 6); // Right white knight
    m_tabpiBoard[62]->movePiece(m_tabpiBoard, 62); // Left black knight
    m_tabpiBoard[57]->movePiece(m_tabpiBoard, 57); // Right black knight

    for (int iTabPosition = 8; iTabPosition < 16; ++iTabPosition) {
        int iTabBlackPawnPosition = 55 + (8 - iTabPosition);
        m_tabpiBoard[iTabPosition]->movePiece(m_tabpiBoard, iTabPosition);
        m_tabpiBoard[iTabBlackPawnPosition]->movePiece(m_tabpiBoard, iTabBlackPawnPosition);
    }
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

bool Board::isKingInCheck(Color in_kingColor) const
{
    if(in_kingColor == Color::WHITE)
    {
        return m_isWhiteKingChecked;
    }
    return m_isBlackKingChecked;
}



bool Board::isMovementPossible(int in_iStartPosition, int in_iTargetPosition) const
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

    std::vector<int> itabValidPositions = pPieceToSeeValidMove->getPossibleMoves();

    for(int iIndiceValidPosition = 0; iIndiceValidPosition < itabValidPositions.size(); iIndiceValidPosition++)
    {
        if(itabValidPositions[iIndiceValidPosition] == in_iTargetPosition)
        {
            return true;
        }
    }

    return false;
}

bool Board::movePiece(int in_iStartPosition, int in_iEndPosition, Color in_colPlayer)
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

        //pPiece->setAlreadyMoved(true);
        placePiece(in_iEndPosition, pPiece);
        m_tabpiBoard[in_iStartPosition] = nullptr;

        /*if (!wasEnPassant) {
            m_enPassantPosition = Coordinate{-1, -1};
        }*/

        /*Coordinate coordKingEnemy = findKing(pPiece->getEnemyColor());
        if(isCaseAttackedByColor(coordKingEnemy.iRow, coordKingEnemy.iColumn, in_colPlayer))
        {
            if(in_colPlayer == Color::WHITE)
            {
                m_isBlackKingChecked = true;
            }
            else
            {
                m_isWhiteKingChecked = true;
            }
        }*/

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

    /*const int* knightMoves = Piece::getKnightMoves();

    for (const int& move : knightMoves) {
        int knightPosition = in_iPosition + move;
        if (isValidPosition(knightPosition)) {
            if (m_tabpiBoard[knightPosition] == nullptr) {
                in_vectPositionPieceFound.push_back(knightPosition);
            }
        }
    }*/
    const int* knightMoves = Piece::getKnightMoves();
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
    m_tabtabpiBoard[in_iRow][in_iCol] = in_pPiece;
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

Piece* Board::getPieceAt(int in_iRow, int in_iColumn) const
{
    if(in_iRow < 0 || in_iRow >= 8 || in_iColumn < 0 || in_iColumn >= 8)
    {
        return nullptr;
    }
    return m_tabtabpiBoard[in_iRow][in_iColumn];
}

bool Board::movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer)
{
    Piece* pPiece = getPieceAt(in_iStartRow, in_iStartCol);
    if(pPiece == nullptr || pPiece->getColor() != in_colPlayer) //If the player try to move a piece of another color, return false
    {
        return false;
    }

    if (isMoveValid(in_iStartRow, in_iStartCol, in_iEndRow, in_iEndCol, in_colPlayer)) {
        bool wasEnPassant = false;
        if (pPiece->getTypePiece() == TypePieces::PAWN) {
            int direction = (in_colPlayer == Color::WHITE) ? 1 : -1;
            if (m_enPassantPosition.getRow()+direction == in_iEndRow && m_enPassantPosition.getColumn() == in_iEndCol) {
                m_tabtabpiBoard[m_enPassantPosition.getRow()][in_iEndCol] = nullptr;
            }
            else if (std::abs(in_iStartRow - in_iEndRow) == 2 && in_iStartCol == in_iEndCol) {
                wasEnPassant= true;
                m_enPassantPosition = Coordinate(in_iEndRow, in_iEndCol);
            }
            if(in_iEndRow == 0 || in_iEndRow==7) {
                m_tabtabpiBoard[in_iEndRow][in_iEndCol] = nullptr;
                promotePawn(in_iEndRow, in_iEndCol, in_colPlayer, &pPiece);
            }
        }

        bool bKingWentRightForRock = in_iEndCol - in_iStartCol == 2;
        bool bKingWentLeftForRock = in_iEndCol - in_iStartCol == -2;

        if(pPiece->getTypePiece() == TypePieces::KING) // If the king rocked (move of length 2, we move the rook)
        {
            Piece* pRookForRock = nullptr;
            if(bKingWentRightForRock)
            {
                pRookForRock = getPieceAt(in_iStartRow, 7);
                //pRookForRock->setAlreadyMoved(true);
                placePiece(in_iStartRow, 5, pRookForRock);
                m_tabtabpiBoard[in_iStartRow][7] = nullptr;
            }
            else if(bKingWentLeftForRock)
            {
                pRookForRock = getPieceAt(in_iStartRow, 0);
               // pRookForRock->setAlreadyMoved(true);
                placePiece(in_iStartRow, 3, pRookForRock);
                m_tabtabpiBoard[in_iStartRow][0] = nullptr;}
        }

        //pPiece->setAlreadyMoved(true);
        placePiece(in_iEndRow, in_iEndCol, pPiece);
        m_tabtabpiBoard[in_iStartRow][in_iStartCol] = nullptr;

        if (!wasEnPassant) {
            m_enPassantPosition = Coordinate{-1, -1};
        }

        Coordinate coordKingEnemy = findKing(pPiece->getEnemyColor());
        if(isCaseAttackedByColor(coordKingEnemy.iRow, coordKingEnemy.iColumn, in_colPlayer))
        {
            if(in_colPlayer == Color::WHITE)
            {
                m_isBlackKingChecked = true;
            }
            else
            {
                m_isWhiteKingChecked = true;
            }
        }

        return true;
    }

    return false;
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
    std::vector<Coordinate> vectPossibleMoves = possibleMovesForPiece(Coordinate(in_iStartRow, in_iStartCol));
    if(vectPossibleMoves.empty()) // Means that the piece could not move
    {
        return false;
    }
    else
    {
        return isCoordinateInVector(coordTargetPoint, vectPossibleMoves);
    }
}

Coordinate Board::findKing(Color in_colorToFind) const
{
    // Parcours de chaque case
    for(int iRow = 0; iRow < 8; ++iRow)
    {
        for(int iCol = 0; iCol < 8; ++iCol)
        {
            Piece* pPiece = m_tabtabpiBoard[iRow][iCol];

            if(pPiece != nullptr && pPiece->getTypePiece() == TypePieces::KING)
            {
                if(pPiece->getColor() == in_colorToFind)
                {
                    return Coordinate(iRow, iCol); // Retourner le pointeur vers la pièce de type roi
                }
            }
        }
    }

    return {};
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
    std::vector<Coordinate> vectPossibleMovesForKing = possibleMovesForPiece(Coordinate(in_iStartRow, in_iStartCol));
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


bool Board::respectBoardLength(int iRow ,int in_iColumn) const
{
    return iRow >= 0 && iRow < 8 && in_iColumn >= 0 && in_iColumn < 8;
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

bool Board::isVectorsProjectionsAttackingCase(int in_iRow, int in_iColumn, Color in_colorToFindAttack, const Vector* in_tabVectorOfPiece, int in_iNbVector, TypeOfPieceAttack in_typeOfAttackOfTheVector) const
{
    if(!respectBoardLength(in_iRow, in_iColumn) || in_tabVectorOfPiece == nullptr || in_iNbVector <= 0)
    {
        return false;
    }

    int iIndicePieceFound = 0;

    for (int iIndicesVector = 0; iIndicesVector < in_iNbVector; ++iIndicesVector)
    {
        Vector vectPossible = in_tabVectorOfPiece[iIndicesVector];

        Piece* pPieceFound = findFirstPieceOnVector(in_iRow, in_iColumn, vectPossible, iIndicePieceFound);

        if(pPieceFound != nullptr)
        {
            TypeOfPieceAttack typeOfAttackPieceFound = pPieceFound->typeOfAttack();

            bool bGoodTypeOfAttackResearched = doesPieceHaveGoodTypeOfAttack(pPieceFound, in_typeOfAttackOfTheVector);

            if(pPieceFound->getColor() == in_colorToFindAttack && bGoodTypeOfAttackResearched)
            {
                if(pPieceFound->getTypePiece() == TypePieces::PAWN || pPieceFound->getTypePiece() == TypePieces::KING) {
                    return iIndicePieceFound == 1; // Le pion et le roi ne peuvent attaquer qu'à 1 de distance
                }
                return true;
            }
        }
    }

    return false;
}

Piece* Board::findFirstPieceOnVector(int in_iStartRow, int in_iStartCol, Vector& in_vectMove, int& in_iIndicePieceFound) const {
    if(! respectBoardLength(in_iStartRow, in_iStartCol))
    {
        return nullptr;
    }

    int iRowVector = in_vectMove.iRow;
    int iColVector = in_vectMove.iColumn;
    int iLengthVector = in_vectMove.iLength;

    for (int iProgressionVector = 1; iProgressionVector <= iLengthVector; ++iProgressionVector)
    {
        int iNextRow = in_iStartRow + (iRowVector * iProgressionVector);  // The indices of the next row/col (vector applied)
        int iNextCol = in_iStartCol + (iColVector * iProgressionVector);

        bool isNextRowValid = (iNextRow >= 0 && iNextRow < 8);
        bool isNextColValid = (iNextCol >= 0 && iNextCol < 8);

        if (!isNextRowValid || !isNextColValid) // If the next move is out of the board, we stop
        {
            break;
        }

        Piece* pPieceFound = getPieceAt(iNextRow, iNextCol);
        if(pPieceFound != nullptr)
        {
            in_iIndicePieceFound = iProgressionVector;
            return pPieceFound;
        }
    }

    in_iIndicePieceFound = 0;
    return nullptr;
}

bool Board::isCaseAttackedByColor(int in_iRow, int in_iCol, Color in_colorToFindAttack) const
{
    if(! respectBoardLength(in_iRow, in_iCol))
    {
        return false;
    }

    // Rook movement
    Vector* vectOfRook = new Vector[4];
    int iNbRookVector = 4;
    //Piece::getRookVectorWithAdjustableLength(vectOfRook, 0, 7);

    bool bIsCaseAttackedStraight = isVectorsProjectionsAttackingCase(in_iRow, in_iCol, in_colorToFindAttack, vectOfRook, iNbRookVector, TypeOfPieceAttack::STRAIGHT);
    delete[] vectOfRook;


    // Bishop movement
    Vector* vectOfBishop = new Vector[4];
    int iNbBishopVector = 4;
    //Piece::getBishopVectorWithAdjustableLength(vectOfBishop, 0, 7);

    bool bIsCaseAttackedDiagonal = isVectorsProjectionsAttackingCase(in_iRow, in_iCol, in_colorToFindAttack, vectOfBishop, iNbBishopVector, TypeOfPieceAttack::DIAGONAL);
    delete[] vectOfBishop;


    // Knight movement
    Vector* vectOfKnight = new Vector[8];
    int iNbKnightVector = 8;
    //Piece::getKnightVectorWithAdjustableLength(vectOfKnight, 0, 1);

    bool bIsCaseAttackedL = isVectorsProjectionsAttackingCase(in_iRow, in_iCol, in_colorToFindAttack, vectOfKnight, iNbKnightVector, TypeOfPieceAttack::L);
    delete[] vectOfKnight;

    return bIsCaseAttackedStraight || bIsCaseAttackedDiagonal || bIsCaseAttackedL;
}

bool Board::isTherePiecesBetweenKingAndRookNotMoving(Vector& in_vect, int in_iRowStart, int in_iColumnStart) const
{
    if(! respectBoardLength(in_iRowStart, in_iColumnStart))
    {
        return false;
    }

    int iUseless = 0;
    Piece* pPieceFound = findFirstPieceOnVector(in_iRowStart, in_iColumnStart, in_vect, iUseless);

    if(pPieceFound != nullptr && pPieceFound->getTypePiece() == TypePieces::ROOK /*&& pPieceFound->hasAlreadyMoved() == false*/) // If it is a Rook that has never moved
    {
        return true;
    }
    return false;
}



void Board::putNextMoveIfValid(Coordinate& in_coordKing, bool in_isKingInCheck, Coordinate& in_coordNextMove, Piece* in_pPieceToMove, Piece* pPieceFoundOnNextMove, std::vector<Coordinate>& in_vectMoveToFill)
{
    int iNextRow = in_coordNextMove.iRow;
    int iNextCol = in_coordNextMove.iColumn;

    //if(in_isKingInCheck)
    //{
        m_tabtabpiBoard[iNextRow][iNextCol] = in_pPieceToMove;
        if(!isCaseAttackedByColor(in_coordKing.iRow, in_coordKing.iColumn, in_pPieceToMove->getEnemyColor()))
        {
            in_vectMoveToFill.emplace_back(iNextRow, iNextCol);
        }
        m_tabtabpiBoard[iNextRow][iNextCol] = pPieceFoundOnNextMove; // We put it back to normal
    //}
    /*else
    {
        in_vectMoveToFill.emplace_back(iNextRow, iNextCol);
    }*/
}

std::vector<Coordinate> Board::getMovementsPossibleWithVector(int in_iStartRow, int in_iStartCol, Vector& in_vectMove)
{
    //TODO Verify if moving cause the king to be check or not
    if(! respectBoardLength(in_iStartRow, in_iStartCol))
    {
        return {};
    }

    Piece* pPieceToSeeValidMove = getPieceAt(in_iStartRow, in_iStartCol);
    if(pPieceToSeeValidMove == nullptr)
    {
        return {};
    }

    Color colEnemyPieces = pPieceToSeeValidMove->getEnemyColor();
    Color colPieceToSeeValidMove = pPieceToSeeValidMove->getColor();
    TypePieces typePieceToSeeValidMove = pPieceToSeeValidMove->getTypePiece();

    int iRowVector = in_vectMove.iRow;
    int iColVector = in_vectMove.iColumn;
    int iLengthVector = in_vectMove.iLength;

    bool isKingChecked = isKingInCheck(colPieceToSeeValidMove);
    Coordinate coordKing = findKing(colPieceToSeeValidMove);

    std::vector<Coordinate> vectMovePossible; // We store all the movements possible

    for (int iProgressionVector = 1; iProgressionVector <= iLengthVector; ++iProgressionVector)
    {
        int iNextRow = in_iStartRow + (iRowVector * iProgressionVector);  // The indices of the next row/col (vector applied)
        int iNextCol = in_iStartCol + (iColVector * iProgressionVector);
        Coordinate coordNextMove = Coordinate(iNextRow, iNextCol);

        bool isNextRowValid = (iNextRow >= 0 && iNextRow < 8);
        bool isNextColValid = (iNextCol >= 0 && iNextCol < 8);

        if (!isNextRowValid || !isNextColValid) // If the next move is out of the board, we stop
        {
            break;
        }

        Piece* pPieceFound = getPieceAt(iNextRow, iNextCol);
        //if(isKingChecked)
        //{
            m_tabtabpiBoard[in_iStartRow][in_iStartCol] = nullptr;
        //}
        if (typePieceToSeeValidMove == TypePieces::PAWN)
        {
            if (iColVector == 0 && pPieceFound == nullptr)
            {
                //vectMovePossible.emplace_back(iNextRow, iNextCol);
                putNextMoveIfValid(coordKing, isKingChecked, coordNextMove, pPieceToSeeValidMove, pPieceFound,vectMovePossible);
                continue;
            }
            else if (iColVector != 0 && pPieceFound != nullptr && pPieceFound->getColor() != colPieceToSeeValidMove)
            {
                //vectMovePossible.emplace_back(iNextRow, iNextCol);
                putNextMoveIfValid(coordKing, isKingChecked, coordNextMove, pPieceToSeeValidMove, pPieceFound,vectMovePossible);
                continue;
            }
            else
            {
                break;
            }
        }
        else if(typePieceToSeeValidMove == TypePieces::KING)
        {
            if(pPieceFound != nullptr && pPieceFound->getColor() != colPieceToSeeValidMove && iProgressionVector == 1 && ! isCaseAttackedByColor(iNextRow, iNextCol, colEnemyPieces)) // If the is a piece he can eat, If that piece does not put the king in check
            {
                vectMovePossible.emplace_back(iNextRow, iNextCol);
            }
            else if(pPieceFound == nullptr && ! isCaseAttackedByColor(iNextRow, iNextCol, colEnemyPieces)) // If the case is empty and If the move does not put the king in check
            {
                if(iProgressionVector == 1)
                {
                    vectMovePossible.emplace_back(iNextRow, iNextCol);
                }
                else if(iProgressionVector == 2 && /*! pPieceToSeeValidMove->hasAlreadyMoved() &&*/ isTherePiecesBetweenKingAndRookNotMoving(in_vectMove, iNextRow, iNextCol) && ! isKingChecked) // For the rock
                {
                    vectMovePossible.emplace_back(iNextRow, iNextCol);
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
        else if (pPieceFound != nullptr) // If a piece is found
        {
            Color colPieceFound = pPieceFound->getColor();
            TypePieces typePieceFound = pPieceFound->getTypePiece();

            // TODO /!\ Verify if the king is not moving and then put in check
            if (colPieceToSeeValidMove != colPieceFound && typePieceFound != TypePieces::KING) // We can't eat the same Color or eat a King
            {
                //vectMovePossible.push_back(Coordinate(iNextRow, iNextCol));
                putNextMoveIfValid(coordKing, isKingChecked, coordNextMove, pPieceToSeeValidMove, pPieceFound,vectMovePossible);
            }
            iProgressionVector = iLengthVector+1; //TODO Réellement utile ?
            break; // Stop since we hit a piece
        }

        //vectMovePossible.push_back(Coordinate(iNextRow, iNextCol)); // Possible to move, we store this position
        putNextMoveIfValid(coordKing, isKingChecked, coordNextMove, pPieceToSeeValidMove, pPieceFound,vectMovePossible);
    }

    //if(isKingChecked)
    //{
        m_tabtabpiBoard[in_iStartRow][in_iStartCol] = pPieceToSeeValidMove; // We put it back because we erased it before
    //}

    return vectMovePossible;
}

// Ajoutez des méthodes pour lister les mouvements possibles
std::vector<Move> Board::listOfPossibleMoves(Color in_colColor)
{
    std::vector<Move> possibleMoves;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* piece = getPieceAt(row, col);
            if (piece != nullptr && piece->getColor() == in_colColor) {
                Coordinate pieceCoord(row, col);
                std::vector<Coordinate> moves = possibleMovesForPiece(pieceCoord);
                for (const Coordinate& move : moves) {
                    possibleMoves.emplace_back(Move{pieceCoord, move});
                }
            }
        }
    }

    return possibleMoves;
}

std::vector<Coordinate> Board::possibleMovesForPiece(const Coordinate& in_coordPiece)
{
    Piece* pPiece = getPieceAt(in_coordPiece);
    if(pPiece == nullptr)
    {
        return {};
    }

    Vector* vectOfPiece = nullptr;
    int iNbVector = 0;
    //pPiece->getVectorOfDisplacement(&vectOfPiece, iNbVector);

    std::vector<Coordinate> vectCoordAllowToMovePiece;
    for (int iIndicesVector = 0; iIndicesVector < iNbVector; ++iIndicesVector)
    {
        Vector vectPossible = vectOfPiece[iIndicesVector];

        // Récupérer les mouvements possibles
        std::vector<Coordinate> vectCoordMovePossible = getMovementsPossibleWithVector(in_coordPiece.iRow, in_coordPiece.iColumn, vectPossible);

        // Vérifier si le vecteur n'est pas vide avant de l'insérer
        if (!vectCoordMovePossible.empty())
        {
            vectCoordAllowToMovePiece.insert(vectCoordAllowToMovePiece.end(), vectCoordMovePossible.begin(), vectCoordMovePossible.end());
        }
    }

    delete[] vectOfPiece; // Properly deallocate memory
    vectOfPiece = nullptr;

    return vectCoordAllowToMovePiece;
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


