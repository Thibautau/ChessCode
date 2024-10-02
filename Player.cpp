//
// Created by Peter on 02/10/2024.
//
#include "Player.h"
#include <iostream>
#include <string>

Player::Player(const Color color)
    : m_colPlayerColor(color), m_iScore(0) {}

Color Player::getPlayerColor() const {
    return m_colPlayerColor;
}

void Player::setPlayerColor(const Color color) {
    m_colPlayerColor = color;
}

int Player::getScore() const {
    return m_iScore;
}

void Player::setScore(const int in_iScore) {
    m_iScore = in_iScore;
}

std::pair<Coordinate, Coordinate> Player::play() {
    std::string input;
    Coordinate start, end;

    do {
        input = getInput();
    } while (!isInputValid(input, start, end));

    return {start, end};
}

std::string Player::getInput() {
    std::string sInput;
    std::cout << "Veuillez entrer votre mouvement (ex: 'e2 e4'): ";
    std::getline(std::cin, sInput);
    return sInput;
}

bool Player::isInputValid(const std::string& in_sInput, Coordinate& out_start, Coordinate& out_end) {
    if (in_sInput.length() != 5 || in_sInput[2] != ' ') {
        std::cout << "Erreur: L'entrée doit être au format 'e2 e4'." << std::endl;
        return false;
    }

    char startCol = in_sInput[0];
    char startRow = in_sInput[1];
    char endCol = in_sInput[3];
    char endRow = in_sInput[4];

    if (!isValidCoordinate(startCol, startRow) || !isValidCoordinate(endCol, endRow)) {
        return false;
    }

    out_start = {startRow - '1', startCol - 'a'};
    out_end = {endRow - '1', endCol - 'a'};

    return true;
}


bool Player::isValidCoordinate(char col, char row) {
    if (col < 'a' || col > 'h') {
        std::cout << "Erreur: La colonne doit être entre 'a' et 'h'." << std::endl;
        return false;
    }

    if (row < '1' || row > '8') {
        std::cout << "Erreur: La ligne doit être entre '1' et '8'." << std::endl;
        return false;
    }

    return true;
}

