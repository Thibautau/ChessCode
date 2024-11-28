//
// Created by Peter on 02/10/2024.
//

#ifndef PLAYERHUMAN_H
#define PLAYERHUMAN_H

#include "Type.h"
#include "Board.h"
#include "Player.h"
#include <string>


class PlayerHuman : public Player {
protected:
    Color m_colPlayerColor;

public:
    PlayerHuman(Color color);
    /**
    * Récupère la couleur du joueur.
    *
    * @return La couleur du joueur.
    */
    Color getPlayerColor() const override;
    /**
    * Définit la couleur du joueur.
    *
    * @param color La couleur à attribuer au joueur.
    */
    void setPlayerColor(Color color) override;
    /**
    * Demande au joueur de saisir un mouvement et valide l'entrée.
    * Le mouvement est récupéré sous forme d'une chaîne de caractères,
    * et une boucle continue jusqu'à ce que l'entrée soit valide.
    *
    * @param board Le plateau de jeu (utilisé pour valider le mouvement).
    * @param start La position de départ du mouvement.
    * @param end La position d'arrivée du mouvement.
    */
    void play(Board& board, int& start, int& end) override;
    /**
    * Demande au joueur d'entrer un mouvement sous forme de chaîne de caractères.
    *
    * @param input La chaîne de caractères contenant le mouvement saisi par le joueur.
    */
    static void getInput(std::string& input);
    /**
    * Vérifie si l'entrée du joueur est valide.
    * Un mouvement est valide si sa longueur est de 4 caractères et si les coordonnées sont correctes.
    *
    * @param in_sInput La chaîne de caractères représentant le mouvement du joueur.
    * @param out_start La position de départ (sortie de la fonction).
    * @param out_end La position d'arrivée (sortie de la fonction).
    * @return True si l'entrée est valide, sinon false.
    */
    static bool isInputValid(const std::string& in_sInput, int& out_start, int& out_end);
    /**
    * Vérifie si une coordonnée est valide.
    * Les colonnes doivent être entre 'a' et 'h' et les lignes entre '1' et '8'.
    *
    * @param col La colonne à vérifier.
    * @param row La ligne à vérifier.
    * @return True si la coordonnée est valide, sinon false.
    */
    static bool isValidCoordinate(char col, char row);
};

#endif // PLAYERHUMAN_H
