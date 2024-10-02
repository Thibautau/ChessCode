//
// Created by Peter on 02/10/2024.
//

#include "Board.h"
#include <iostream>

Board::Board()
{
    for(int iIndiceRow = 0; iIndiceRow < 8; iIndiceRow++ )
    {
        for(int iIndicesColumn = 0; iIndicesColumn < 8; iIndicesColumn++ )
        {
            m_tabtabpiBoard[iIndiceRow][iIndicesColumn] = nullptr;
        }
    }
}

void Board::initializeBoard()
{
    for (int iIndicesCol = 0; iIndicesCol < 8; ++iIndicesCol) {
        placePiece(1, iIndicesCol, new Piece(TypePieces::PAWN, Color::WHITE));
        placePiece(6, iIndicesCol, new Piece(TypePieces::PAWN, Color::BLACK));
    }

    placePiece(0, 0, new Piece(TypePieces::ROOK, Color::WHITE));
    placePiece(0, 1, new Piece(TypePieces::KNIGHT, Color::WHITE));
    placePiece(0, 2, new Piece(TypePieces::BISHOP, Color::WHITE));
    placePiece(0, 3, new Piece(TypePieces::QUEEN, Color::WHITE));
    placePiece(0, 4, new Piece(TypePieces::KING, Color::WHITE));
    placePiece(0, 5, new Piece(TypePieces::BISHOP, Color::WHITE));
    placePiece(0, 6, new Piece(TypePieces::KNIGHT, Color::WHITE));
    placePiece(0, 7, new Piece(TypePieces::ROOK, Color::WHITE));

    placePiece(7, 0, new Piece(TypePieces::ROOK, Color::BLACK));
    placePiece(7, 1, new Piece(TypePieces::KNIGHT, Color::BLACK));
    placePiece(7, 2, new Piece(TypePieces::BISHOP, Color::BLACK));
    placePiece(7, 3, new Piece(TypePieces::QUEEN, Color::BLACK));
    placePiece(7, 4, new Piece(TypePieces::KING, Color::BLACK));
    placePiece(7, 5, new Piece(TypePieces::BISHOP, Color::BLACK));
    placePiece(7, 6, new Piece(TypePieces::KNIGHT, Color::BLACK));
    placePiece(7, 7, new Piece(TypePieces::ROOK, Color::BLACK));
}

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

bool Board::movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol)
{
    Piece* pPiece = getPieceAt(in_iStartRow, in_iStartCol);
    if(pPiece == nullptr) {
        return false;
    }

    if (isMoveValid(in_iStartRow, in_iStartCol, in_iEndRow, in_iEndCol)) {
        placePiece(in_iEndRow, in_iEndCol, pPiece);
        m_tabtabpiBoard[in_iStartRow][in_iStartCol] = nullptr;
    }

    return true;
}

bool Board::isMoveValid(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol) const
{
    if(in_iStartRow >= 8 || in_iStartCol >= 8 || in_iStartRow < 0 || in_iStartCol < 0
        || in_iEndRow >= 8 || in_iEndCol >= 8 || in_iEndRow < 0 || in_iEndCol < 0)
    {
        return false;
    }

    Piece* pPieceToMove = getPieceAt(in_iStartRow, in_iStartCol);
    if(pPieceToMove == nullptr) // If we didn't found the piece to move
    {
        return false;
    }

    Piece* pPieceToEndPoint = getPieceAt(in_iEndRow, in_iEndCol);
    if(pPieceToEndPoint == nullptr) // If the end point is free, we can move
    {
        return true;
    }
    else if(pPieceToMove->getColor() == pPieceToEndPoint->getColor()) // If the color are the same, we can't eat, so we can't move
    {
        return false;
    }


    return true;
}

std::vector<Coordinate> Board::getMovementsPossibleWithVector(int in_iStartRow, int in_iStartCol, Vector& in_vectMove) const
{
    //TODO Verify if moving cause the king to be check or not
    if(in_iStartRow >= 8 || in_iStartCol >= 8 || in_iStartRow < 0 || in_iStartCol < 0)
    {
        return {};
    }

    Piece* pPieceToSeeValidMove = getPieceAt(in_iStartRow, in_iStartCol);
    if(pPieceToSeeValidMove == nullptr)
    {
        return {};
    }

    Color colPieceToSeeValidMove = pPieceToSeeValidMove->getColor();

    int iRowVector = in_vectMove.iRow;
    int iColVector = in_vectMove.iColumn;
    int iLengthVector = in_vectMove.iLength;

    Piece* pPieceFound = nullptr; // The piece or nullptr on the path of the vector for an iteration
    bool isNextRowValid, isNextColValid; // To know if the next row/col (vector applied) is in the board area (0-7, 0-7)
    int iNextRow, iNextCol; // The indices of the next row/col (vector applied)
    int iProgressionVector = 1; // Indice to multiply the vector onto

    std::vector<Coordinate> vectMovePossible; // We store all the movements possible

    do // We visit all the cases between the start point and the furthest possible point
    {
        iNextRow = in_iStartRow + (iRowVector * iProgressionVector);
        iNextCol = in_iStartCol + (iColVector * iProgressionVector);

        isNextRowValid = (iNextRow >= 0 && iNextRow < 8);
        isNextColValid = (iNextCol >= 0 && iNextCol < 8);

        pPieceFound = getPieceAt(iNextRow, iNextCol);
        if(pPieceFound != nullptr)
        {
            Color colPieceFound = pPieceFound->getColor();
            // Si les 2 pièces ont la même couleur, elle ne peut pas aller plus loin. We can't eat the king
            if(colPieceToSeeValidMove == colPieceFound || pPieceFound->getTypePiece() == TypePieces::KING)
            {
                iProgressionVector--; // -- because we can't reach the next case, we move back
                vectMovePossible.insert(vectMovePossible.end(), Coordinate(in_iStartRow + (iRowVector * (iProgressionVector -1)), in_iStartCol + (iColVector * (iProgressionVector - 1))));
            }
            else // TODO /!\ Verify if the king is not moving and then put in check
            {
                // If we can eat, we add this move to those possible
                vectMovePossible.insert(vectMovePossible.end(), Coordinate(iNextRow, iNextCol));
            }
            break; // We get out because we know the furthest case for this vector
        }

        // If we can move, we add this to those possible
        vectMovePossible.insert(vectMovePossible.end(), Coordinate(iNextRow, iNextCol));
        iProgressionVector++;
    }while(iProgressionVector <= iLengthVector && isNextRowValid && isNextColValid);

    return vectMovePossible;
}

// Ajoutez des méthodes pour lister les mouvements possibles
std::vector<Move> Board::listOfPossibleMoves(Color in_colColor) const
{
    return {};
}

std::vector<Coordinate> Board::possibleMovesForPiece(const Coordinate& in_coordPiece) const
{
    Piece* pPiece = getPieceAt(in_coordPiece);
    if(pPiece == nullptr)
    {
        return {};
    }

    Vector* vectOfPiece = nullptr;
    int iNbVector = 0;
    pPiece->getVectorOfDisplacement(&vectOfPiece, iNbVector);

    std::cout << "Type: " << pPiece->getDisplayChar() << " , Color: " << pPiece->getColorAsChar() << std::endl;


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

    return vectCoordAllowToMovePiece;
}

void Board::displayBoard() const {
    for (int row = 7; row >= 0; --row) {
        std::cout << row + 1 << " ";
        for (int col = 0; col < 8; ++col) {
            Piece* piece = m_tabtabpiBoard[row][col];
            if (piece) {
                char displayChar = piece->getDisplayChar();
                // Vérifier la couleur de la pièce
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

    std::cout << "  a b c d e f g h" << std::endl;
}


