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

void Board::placePiece(int in_iRow, int in_iCol, Piece* in_pPiece)
{
    m_tabtabpiBoard[in_iRow][in_iCol] = in_pPiece;
}

Piece* Board::getPieceAt(const Coordinate& coord) const
{
    return m_tabtabpiBoard[coord.iRow][coord.iColumn];
}

Piece* Board::getPieceAt(int in_iRow, int in_iColumn) const
{
    return m_tabtabpiBoard[in_iRow][in_iColumn];
}

bool Board::movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol) {
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

bool Board::isMoveValid(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol) const {
    return true;
}

// Ajoutez des méthodes pour lister les mouvements possibles
std::vector<Move> Board::listOfPossibleMoves(Color in_colColor) const {
    return {};
}

std::vector<Move> Board::possibleMovesForPiece(const Coordinate& in_coordPiece) const {
    return {};
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


