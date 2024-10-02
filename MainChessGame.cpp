//
// Created by Peter on 02/10/2024.
//

#include "MainChessGame.h"
#include "Player.h"
#include <iostream>

MainChessGame::MainChessGame()
{
    m_currentPlayer = new Player(Color::WHITE);
    m_waitingPlayer = new Player(Color::BLACK);
    m_board = new Board();
    m_isGameOver = false;
    m_colorWinner = Color::NONE;
}


void MainChessGame::initChessGame() const
{
    m_board->initializeBoard();
}

void MainChessGame::playTurn()
{
    std::cout << "C'est au tour de " << (m_currentPlayer->getPlayerColor() == Color::WHITE ? "Blanc" : "Noir") << std::endl;

    m_board->displayBoard();

    std::pair<Coordinate, Coordinate> move = m_currentPlayer->play();

    Coordinate coordStart = move.first;
    Coordinate coorEnd = move.second;

    m_board->displayBoard();


    if (m_board->isMoveValid(coordStart.iRow, coordStart.iColumn, coorEnd.iRow, coorEnd.iColumn)) {
        m_board->movePiece(coordStart.iRow, coordStart.iColumn, coorEnd.iRow, coorEnd.iColumn);
        changeCurrentPlayer();
    } else {
        std::cout << "Mouvement invalide. Essayez encore." << std::endl;
    }
}

void MainChessGame::changeCurrentPlayer()
{
    std::swap(m_currentPlayer, m_waitingPlayer);
}

bool MainChessGame::isGameOver() const
{
    return m_isGameOver;
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


