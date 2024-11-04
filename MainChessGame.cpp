//
// Created by Peter on 02/10/2024.
//

#include "MainChessGame.h"
#include "Player.h"
#include "PlayerHuman.h"
#include "Bot.h"
#include <sstream>
#include <cctype>
#include <iostream>

MainChessGame::MainChessGame(GameMode mode)
{
    switch (mode) {
        case GameMode::JVJ:
            m_currentPlayer = new PlayerHuman(Color::WHITE);
            m_waitingPlayer = new PlayerHuman(Color::BLACK);
        break;

        case GameMode::JVB:
            m_currentPlayer = new PlayerHuman(Color::WHITE);
            m_waitingPlayer = new Bot(Color::BLACK);
        break;

        case GameMode::BVB:
            m_currentPlayer = new Bot(Color::WHITE);
            m_waitingPlayer = new Bot(Color::BLACK);
        break;

        default:
            throw std::invalid_argument("Mode de jeu invalide.");
    }
    m_board = new Board();
    m_isGameOver = false;
    m_colorWinner = Color::NONE;
}


void MainChessGame::initChessGame() const
{
    m_board->initializeBoard();
}

void MainChessGame::playTurn(const std::string& move, char promotion)
{
    std::cout << "C'est au tour de " << (m_currentPlayer->getPlayerColor() == Color::WHITE ? "Blanc" : "Noir") << std::endl;

    //m_board->displayBoard();

    if (move.empty()) {
        m_board->displayBoard();
        int coordStart;
        int coordEnd;
        m_currentPlayer->play(*m_board, coordStart, coordEnd);
        if (m_board->movePiece(coordStart, coordEnd, m_currentPlayer->getPlayerColor()))
        {
            changeCurrentPlayer();
        }
        else
        {
            std::cout << "Mouvement invalide. Essayez encore." << std::endl;
        }
    }
    else {
        if (m_board->movePiece(move,m_currentPlayer->getPlayerColor(),Piece::charToPieceType(promotion)))
        {
            changeCurrentPlayer();
            m_board->displayBoard();
        }
        else
        {
            std::cout << "Mouvement invalide. Essayez encore." << std::endl;
        }

    }

    //debugPrintMove(coordStart, coordEnd);

    //std::cout << "CoordStart:" << coordStart << " | coordEnd: " << coordEnd << "\n";

}

std::string MainChessGame::indexToPosition(int pos) {
    int row = pos / 8;
    int col = pos % 8;

    char columnChar = 'a' + col;
    char rowChar = '1' + row;

    return std::string{columnChar} + rowChar;
}

void MainChessGame::setBoardFromFEN(const std::string& fen) {
    m_board->setupFromFEN(fen);

    std::istringstream fenStream(fen);
    std::string boardPart, activeColor, castling, enPassant, halfMoveClock, fullMoveNumber;

    // Divise les parties de la notation FEN
    fenStream >> boardPart >> activeColor >> castling >> enPassant >> halfMoveClock >> fullMoveNumber;

    // Vérifier et configurer le joueur actif
    Color currentPlayerColor = m_currentPlayer->getPlayerColor();
    Color newActiveColor = (activeColor == "w") ? Color::WHITE : Color::BLACK;

    if (currentPlayerColor != newActiveColor) {
        changeCurrentPlayer();
    }

    // Configure les droits de roque
    m_board->setCastlingRightsForFenNotation(castling);

    // Configure la position pour la prise en passant, si applicable
    if (enPassant != "-") {
        int enPassantPos = Board::convertToPosition(enPassant[0], enPassant[1]);
        m_board->setEnPassantPosition(enPassantPos);
    }
    else
    {
        m_board->setEnPassantPosition(-1); // Valeur par défaut si aucune prise en passant n'est possible
    }
}

std::pair<int, int> MainChessGame::findBestMoveForCurrentPlayer(int depth) {
    int iRow, iCol;

    Bot* botPlayer = dynamic_cast<Bot*>(m_currentPlayer);
    if(depth > 0 || botPlayer != nullptr) {
        botPlayer->playWithDepth(*m_board, iRow, iCol, depth);
    }
    else {
        m_currentPlayer->play(*m_board, iRow, iCol);
    }
    return {iRow, iCol};
}



void MainChessGame::debugPrintMove(int start, int end) {
    std::cout << "Deplacement de " << indexToPosition(start) << " a " << indexToPosition(end) << std::endl;
}

void MainChessGame::changeCurrentPlayer()
{
    std::swap(m_currentPlayer, m_waitingPlayer);
}

bool MainChessGame::isGameOver()
{
    return m_board->isGameOver(m_currentPlayer->getPlayerColor(), m_colorWinner);
}

Color MainChessGame::getColorWinner() const
{
    return m_colorWinner;
}

Player* MainChessGame::getCurrentPlayer() const
{
    return m_currentPlayer;
}

Player* MainChessGame::getWaitingPlayer() const
{
    return m_waitingPlayer;
}

Board* MainChessGame::getBoard() const
{
    return m_board;
}

MainChessGame::~MainChessGame() {
    delete m_currentPlayer;
    delete m_waitingPlayer;
    delete m_board;
    m_currentPlayer= nullptr;
    m_waitingPlayer= nullptr;
    m_board= nullptr;
}

Piece* MainChessGame::getPieceAt(const std::string& in_sPosition) const
{
    int iStartPos = convertToPosition(in_sPosition[0], in_sPosition[1]);
    return m_board->getPieceAt(iStartPos);
}

int MainChessGame::convertToPosition(char col, char row) const
{
    int column = col - 'a';
    int line = row - '1';
    return column + line * 8;
}
