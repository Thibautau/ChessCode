// Created by Peter on 02/10/2024.

#include "PlayerHuman.h"
#include <iostream>
#include <string>

PlayerHuman::PlayerHuman(Color color)
    : m_colPlayerColor(color) {}

Color PlayerHuman::getPlayerColor() const {
    return m_colPlayerColor;
}

void PlayerHuman::setPlayerColor(Color color) {
    m_colPlayerColor = color;
}

void PlayerHuman::play(Board& board,int& start, int& end) {
    std::string input;

    do {
        getInput(input);
    } while (!isInputValid(input, start, end));
}

void PlayerHuman::getInput(std::string& input) {
    std::cout << "Veuillez entrer votre mouvement (ex: 'e2e4'): ";
    std::getline(std::cin, input);
}

bool PlayerHuman::isInputValid(const std::string& in_sInput, int& out_start, int& out_end) {
    if (in_sInput.length() != 4) {
        std::cout << in_sInput;
        std::cout << "Erreur: L'entree doit etre au format 'e2e4'." << std::endl;
        return false;
    }

    char startCol = in_sInput[0];
    char startRow = in_sInput[1];
    char endCol = in_sInput[2];
    char endRow = in_sInput[3];

    if (!isValidCoordinate(startCol, startRow) || !isValidCoordinate(endCol, endRow)) {
        return false;
    }

    out_start = (startRow - '1')*8 + (startCol - 'a');
    out_end = (endRow - '1')*8 + (endCol - 'a');

    return true;
}

bool PlayerHuman::isValidCoordinate(char col, char row) {
    if (col < 'a' || col > 'h') {
        std::cout << "Erreur: La colonne doit etre entre 'a' et 'h'." << std::endl;
        return false;
    }

    if (row < '1' || row > '8') {
        std::cout << "Erreur: La ligne doit etre entre '1' et '8'." << std::endl;
        return false;
    }

    return true;
}
