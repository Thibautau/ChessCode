//
// Created by Peter on 02/10/2024.
//

#include "MainChessGame.h"
#include "Player.h"
#include <iostream>

MainChessGame::MainChessGame() {
    m_currentPlayer = new Player(Color::WHITE);
    m_waitingPlayer = new Player(Color::BLACK);
    m_board = new Board();
    m_isGameOver = false;
    m_winner = Color::NONE;
}


void MainChessGame::initChessGame() {
    m_board->initializeBoard();
}

void MainChessGame::playTurn() {
    std::cout << "C'est au tour de " << (m_currentPlayer->getPlayerColor() == Color::WHITE ? "Blanc" : "Noir") << std::endl;

    m_board->displayBoard();

    Coordinate start = m_currentPlayer->play();
    Coordinate end = m_currentPlayer->play();

    m_board->displayBoard();


    if (m_board->isMoveValid(start.iRow, start.iColumn, end.iRow, end.iColumn)) {
        m_board->movePiece(start.iRow, start.iColumn, end.iRow, end.iColumn);
        changeCurrentPlayer();
    } else {
        std::cout << "Mouvement invalide. Essayez encore." << std::endl;
    }
}

void MainChessGame::changeCurrentPlayer() {
    std::swap(m_currentPlayer, m_waitingPlayer);
}

bool MainChessGame::isGameOver() {
    return m_isGameOver;
}

Color MainChessGame::getWinner() {
    return m_winner;
}

Player& MainChessGame::getCurrentPlayer() {
    return *m_currentPlayer;
}

Player& MainChessGame::getWaitingPlayer() {
    return *m_waitingPlayer;
}

Board& MainChessGame::getBoard() {
    return *m_board;
}


