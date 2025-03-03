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

        case GameMode::BVJ:
            m_currentPlayer = new Bot(Color::WHITE);
            m_waitingPlayer = new PlayerHuman(Color::BLACK);
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


void MainChessGame::initChessGame()
{
    m_board->initializeBoard();

    if(m_waitingPlayer->getPlayerColor() == Color::WHITE)
    {
        changeCurrentPlayer();
    }
}

void MainChessGame::switchPlayers() {
    std::swap(m_currentPlayer, m_waitingPlayer);

    m_currentPlayer->setPlayerColor(Color::WHITE);
    m_waitingPlayer->setPlayerColor(Color::BLACK);
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
            std::cout << "Mouvement invalide. Essayez encore.1" << std::endl;
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
            std::string sCol = m_currentPlayer->getPlayerColor() == Color::WHITE? "Blanc" : "Noir";
            std::cout << "Mouvement invalide. Essayez encore.2" << move << sCol << std::endl;
        }

    }
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

void MainChessGame::setBoardFromFENStatic(const std::string& fen, Board* in_pBoard) {
    in_pBoard->setupFromFEN(fen);

    std::istringstream fenStream(fen);
    std::string boardPart, activeColor, castling, enPassant, halfMoveClock, fullMoveNumber;

    // Divise les parties de la notation FEN
    fenStream >> boardPart >> activeColor >> castling >> enPassant >> halfMoveClock >> fullMoveNumber;

    // Vérifier et configurer le joueur actif
    Color newActiveColor = (activeColor == "w") ? Color::WHITE : Color::BLACK;

    // Configure les droits de roque
    in_pBoard->setCastlingRightsForFenNotation(castling);

    // Configure la position pour la prise en passant, si applicable
    if (enPassant != "-") {
        int enPassantPos = Board::convertToPosition(enPassant[0], enPassant[1]);
        in_pBoard->setEnPassantPosition(enPassantPos);
    }
    else
    {
        in_pBoard->setEnPassantPosition(-1); // Valeur par défaut si aucune prise en passant n'est possible
    }
}

std::string MainChessGame::findBestMoveForCurrentPlayer(int depth) {
    int start, end;
    char promotion='\0';

    Bot* botPlayer = dynamic_cast<Bot*>(m_currentPlayer);
    if(depth > 0 && botPlayer != nullptr) {

        botPlayer->playWithDepth(*m_board, start, end, depth, promotion);
    }
    std::string move = indexToPosition(start);
    std::string move2 = indexToPosition(end);
    move += move2;
    std::cout << "promotion mainChessGame: " << promotion << "\n";
    if (promotion != '\0') {
        move +=std::string(1, promotion);
    }
    std::cout << "move mainChessGame: " << move << "\n";
    return move;
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
    int iStartPos = Board::convertToPosition(in_sPosition[0], in_sPosition[1]);
    return m_board->getPieceAt(iStartPos);
}
