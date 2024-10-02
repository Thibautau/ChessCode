//
// Created by Peter on 02/10/2024.
//

#include "Board.h"
#include <iostream>

Board::Board() {
    m_tabTabPiBoard.resize(8, std::vector<std::unique_ptr<Piece>>(8, nullptr));
}

void Board::initializeBoard() {
    for (int col = 0; col < 8; ++col) {
        placePiece(1, col, std::make_unique<Piece>(TypePieces::PAWN, Color::WHITE));
        placePiece(6, col, std::make_unique<Piece>(TypePieces::PAWN, Color::BLACK));
    }

    placePiece(0, 0, std::make_unique<Piece>(TypePieces::ROOK, Color::WHITE));
    placePiece(0, 1, std::make_unique<Piece>(TypePieces::KNIGHT, Color::WHITE));
    placePiece(0, 2, std::make_unique<Piece>(TypePieces::BISHOP, Color::WHITE));
    placePiece(0, 3, std::make_unique<Piece>(TypePieces::QUEEN, Color::WHITE));
    placePiece(0, 4, std::make_unique<Piece>(TypePieces::KING, Color::WHITE));
    placePiece(0, 5, std::make_unique<Piece>(TypePieces::BISHOP, Color::WHITE));
    placePiece(0, 6, std::make_unique<Piece>(TypePieces::KNIGHT, Color::WHITE));
    placePiece(0, 7, std::make_unique<Piece>(TypePieces::ROOK, Color::WHITE));

    placePiece(7, 0, std::make_unique<Piece>(TypePieces::ROOK, Color::BLACK));
    placePiece(7, 1, std::make_unique<Piece>(TypePieces::KNIGHT, Color::BLACK));
    placePiece(7, 2, std::make_unique<Piece>(TypePieces::BISHOP, Color::BLACK));
    placePiece(7, 3, std::make_unique<Piece>(TypePieces::QUEEN, Color::BLACK));
    placePiece(7, 4, std::make_unique<Piece>(TypePieces::KING, Color::BLACK));
    placePiece(7, 5, std::make_unique<Piece>(TypePieces::BISHOP, Color::BLACK));
    placePiece(7, 6, std::make_unique<Piece>(TypePieces::KNIGHT, Color::BLACK));
    placePiece(7, 7, std::make_unique<Piece>(TypePieces::ROOK, Color::BLACK));
}

void Board::placePiece(int row, int col, std::unique_ptr<Piece> piece) {
    m_tabTabPiBoard[row][col] = std::move(piece);
}

Piece* Board::getPieceAt(const Coordinate& coord) const {
    return m_tabTabPiBoard[coord.row][coord.col].get();
}

bool Board::movePiece(int startRow, int startCol, int endRow, int endCol) {
    Piece* piece = getPieceAt(Coordinate(startRow, startCol));
    if (piece && isMoveValid(startRow, startCol, endRow, endCol)) {
        placePiece(endRow, endCol, std::move(m_tabTabPiBoard[startRow][startCol]));
        m_tabTabPiBoard[startRow][startCol] = nullptr;
        return true;
    }
    return false;
}

bool Board::isMoveValid(int startRow, int startCol, int endRow, int endCol) const {
    return true;
}

// Ajoutez des méthodes pour lister les mouvements possibles
std::vector<Move> Board::listOfPossibleMoves(Color in_colColor) const {
    return {};
}

std::vector<Move> Board::possibleMovesForPiece(const Coordinate& coordPiece) const {
    return {};
}
