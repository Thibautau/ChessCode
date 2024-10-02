//
// Created by Peter on 02/10/2024.
//
#include "Player.h"

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

//@TODO Transformer l'input en Coordinate
//Le play ne doit pas faire le mouvement mais juste donner le mouvement
Coordinate Player::play() {
    //Là on recupère l'input pour ensuite le transformer en nombre ou en quelques chose de
    //comprehensible par notre jeu
    getInput();
    return 11;
}

std::string Player::getInput() {
    //@ TODO Récupérer les inputs console du joueur
}

//En vrai c'est le main qui doit gérer ça
bool Player::isInputValid(const std::string& input) {
    return true;
}

