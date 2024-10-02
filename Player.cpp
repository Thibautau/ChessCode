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

void Player::setScore(const int score) {
    m_iScore = score;
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
    std::string input;
    std::cout << "Veuillez entrer votre move: ";
    std::getline(std::cin, input);
    return input;
}

bool Player::isInputValid(const std::string& input,int& column, int& row) {
    if (input.length() != 2) {
        std::cout << "Erreur: L'entrée doit être au format '{lettre}{numero}'." << std::endl;
        return false;
    }

    char columnChar = input[0];
    char rowChar = input[1];

    column = columnChar - 'a';
    if (column < 0 || column > 7) {
        std::cout << "Erreur: La colonne doit être entre 'a' et 'h'." << std::endl;
        return false;
    }

    row = rowChar - '1';
    if (row < 0 || row > 7) {
        std::cout << "Erreur: La ligne doit être entre '1' et '8'." << std::endl;
        return false;
    }

    return true;
}

