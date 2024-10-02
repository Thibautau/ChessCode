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

Coordinate Player::play() {
    std::string input;
    int column, row;

    do {
        input = getInput();
    } while (!isInputValid(input, column, row));

    return {row, column};
}

std::string Player::getInput() {
    std::string sInput;
    std::cout << "Veuillez entrer votre move: ";
    std::getline(std::cin, sInput);
    return sInput;
}

bool Player::isInputValid(const std::string& in_sInput,int& out_iColumn, int& out_iRow) {
    if (in_sInput.length() != 2) {
        std::cout << "Erreur: L'entrée doit être au format '{lettre}{numero}'." << std::endl;
        return false;
    }

    char columnChar = in_sInput[0];
    char rowChar = in_sInput[1];

    out_iColumn = columnChar - 'a';
    if (out_iColumn < 0 || out_iColumn > 7) {
        std::cout << "Erreur: La colonne doit être entre 'a' et 'h'." << std::endl;
        return false;
    }

    out_iRow = rowChar - '1';
    if (out_iRow < 0 || out_iRow > 7) {
        std::cout << "Erreur: La ligne doit être entre '1' et '8'." << std::endl;
        return false;
    }

    return true;
}

