//
// Created by Peter on 02/10/2024.
//

#ifndef MAINCHESSGAME_H
#define MAINCHESSGAME_H

#include "PlayerHuman.h"
#include "Board.h"
#include "Type.h"

class MainChessGame {
private:
    Player* m_currentPlayer;
    Player* m_waitingPlayer;
    Board* m_board;
    bool m_isGameOver;
    Color m_colorWinner;

public:
    /**
    * Initialise une nouvelle partie en fonction du mode de jeu spécifié.
    *
    * @param mode Le mode de jeu choisi (JVJ, JVB, ou BVB).
    *
    * @throws std::invalid_argument Si un mode de jeu invalide est fourni.
    */
    explicit MainChessGame(GameMode mode);
    ~MainChessGame();

    /**
    * Initialise l'échiquier pour commencer une partie.
    * Configure l'échiquier et vérifie la couleur du joueur actif.
    */
    void initChessGame();
    /**
    * Convertit une position sur l'échiquier (index) en notation classique (ex. "a1").
    *
    * @param pos La position sur l'échiquier, représentée par un entier.
    *
    * @return La position sous forme de chaîne de caractères (ex. "a1").
    */
    std::string indexToPosition(int pos);
    /**
    * Joue un tour pour le joueur actuel.
    * Si un coup est donné en paramètre, il est exécuté, sinon, un coup est choisi par le joueur humain.
    *
    * @param move Le coup sous forme de chaîne de caractères (ex. "e2e4").
    * @param promotion La pièce de promotion si applicable.
    */
    void playTurn(const std::string& move = "", char promotion = 0);
    /**
    * Configure l'échiquier à partir d'une notation FEN.
    *
    * @param fen La chaîne de caractères FEN représentant l'état de l'échiquier.
    */
    void setBoardFromFEN(const std::string& fen);
    /**
    * Trouve le meilleur coup pour le joueur actuel en fonction de la profondeur donnée.
    *
    * @param depth La profondeur de recherche pour l'algorithme Minimax.
    *
    * @return Le meilleur coup sous forme de chaîne de caractères (ex. "e2e4").
    */
    std::string findBestMoveForCurrentPlayer(int depth = -1);
    /**
    * Affiche un message de débogage pour un déplacement de pièce.
    *
    * @param start La position de départ de la pièce.
    * @param end La position d'arrivée de la pièce.
    */
    void debugPrintMove(int start, int end);
    /**
    * Change le joueur actuel et le joueur en attente.
    * Permet de passer d'un joueur à l'autre à chaque tour.
    */
    void changeCurrentPlayer();
    /**
    * Vérifie si la partie est terminée.
    *
    * @return True si la partie est terminée, sinon False.
    */
    bool isGameOver();
    /**
    * Retourne la couleur du joueur gagnant, si applicable.
    *
    * @return La couleur du joueur gagnant (Color::NONE si la partie n'est pas terminée ou il n'y a pas de gagnant).
    */
    Color getColorWinner() const;
    /**
    * Retourne le joueur actuel.
    *
    * @return Le joueur actuel.
    */
    Player* getCurrentPlayer() const;
    /**
    * Retourne le joueur en attente.
    *
    * @return Le joueur en attente.
    */
    Player* getWaitingPlayer() const;
    /**
    * Retourne l'échiquier.
    *
    * @return L'échiquier de la partie.
    */
    Board* getBoard() const;
    /**
    * Retourne la pièce à une position donnée sur l'échiquier.
    *
    * @param in_sPosition La position sous forme de chaîne (ex. "e2").
    *
    * @return Un pointeur vers la pièce située à cette position, ou nullptr si la position est vide.
    */
    Piece* getPieceAt(const std::string& in_sPosition) const;
};

#endif //MAINCHESSGAME_H
