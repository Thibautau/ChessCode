//
// Created by Peter on 02/10/2024.
//

#ifndef PIECE_H
#define PIECE_H

#define NO_ERROR            0x0000
#define WRONG_PARAMETER     0x0001
#define ERROR               0x0002

#include "Type.h"
#include <vector>
#include <algorithm>
#include <iostream>

class Piece {
private:
    TypePieces m_tpTypePiece;
    Color m_colColorPiece;

public:
    /**
    * Constructeur de la classe Piece.
    * Initialise une pièce avec un type et une couleur spécifiés.
    *
    * @param type Le type de la pièce (par exemple, PAWN, ROOK, etc.).
    * @param color La couleur de la pièce (WHITE ou BLACK).
    */
    Piece(TypePieces type, Color color);
    /**
    * Récupère le type de la pièce.
    *
    * @return Le type de la pièce (par exemple, PAWN, ROOK, etc.).
    */
    TypePieces getTypePiece() const;
    /**
    * Modifie le type de la pièce.
    *
    * @param type Le nouveau type de la pièce (par exemple, PAWN, ROOK, etc.).
    */
    void setTypePiece(TypePieces type);
    /**
    * Récupère la couleur de la pièce.
    *
    * @return La couleur de la pièce (WHITE ou BLACK).
    */
    Color getColor() const;
    /**
    * Récupère la couleur de l'ennemi.
    *
    * @return La couleur de l'ennemi (WHITE ou BLACK).
    */
    Color getEnemyColor() const;
    /**
    * Récupère la couleur de l'ennemi en fonction de la couleur spécifiée.
    *
    * @param in_colPiece La couleur de la pièce pour laquelle obtenir l'ennemi.
    * @return La couleur de l'ennemi (WHITE ou BLACK).
    */
    static Color getEnemyColor(Color in_colPiece);
    /**
    * Récupère la couleur de la pièce sous forme de caractère.
    *
    * @return 'W' pour WHITE, 'B' pour BLACK, et '?' pour une couleur inconnue.
    */
    char getColorAsChar() const;
    /**
    * Récupère le caractère représentant la pièce pour l'affichage.
    *
    * @return Un caractère représentant la pièce (p, r, n, b, q, k pour les pièces de type PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING).
    */
    char getDisplayChar() const;
    /**
    * Convertit un caractère représentant une pièce en type de pièce.
    *
    * @param piece Le caractère représentant une pièce (p, n, b, r, q, k).
    * @return Le type de la pièce correspondant au caractère (par exemple, PAWN, KNIGHT, etc.).
    */
    static TypePieces charToPieceType(char piece);
    /**
    * Récupère les options de promotion disponibles pour une pièce.
    * Les options incluent la promotion en ROOK, KNIGHT, BISHOP, ou QUEEN.
    *
    * @return Un vecteur contenant les types de pièces disponibles pour la promotion.
    */
    static std::vector<TypePieces> pieceForPromotion();
    /**
    * Récupère la colonne de la tour après avoir effectué un roque.
    * Retourne -1 si la colonne spécifiée est invalide.
    *
    * @param in_iColumn La colonne de départ (0-7).
    * @return La colonne de la tour après le roque, ou -1 si la colonne est invalide.
    */
    static int getColumnOfRookAfterRock(int in_iColumn);
    /**
    * Récupère le type d'attaque d'une pièce en fonction de son type.
    *
    * @return Le type d'attaque (DIAGONAL, STRAIGHT, L, etc.).
    */
    TypeOfPieceAttack typeOfAttack() const;
    /**
    * Vérifie si une pièce attaque de manière droite (horizontale ou verticale).
    *
    * @return true si la pièce attaque de manière droite, false sinon.
    */
    bool attackStraight() const;
    /**
    * Vérifie si une pièce attaque de manière diagonale.
    *
    * @return true si la pièce attaque de manière diagonale, false sinon.
    */
    bool attackDiagonal() const;
    /**
    * Vérifie si une pièce attaque en forme de "L" (comme un cavalier).
    *
    * @return true si la pièce attaque en "L", false sinon.
    */
    bool attackKnight() const;

    /**
    * Récupère les déplacements possibles pour le roi.
    * Les déplacements sont retournés dans un tableau d'entiers.
    *
    * @param out_iNbOfRepetitionToDo Nombre de répétitions à effectuer pour chaque direction.
    * @param out_iNbOfMovement Nombre de directions de déplacement.
    * @return Un tableau d'entiers représentant les déplacements possibles du roi.
    */
    static int* getKingMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    /**
    * Récupère les déplacements possibles pour un petit roque.
    *
    * @param out_iNbOfRepetitionToDo Nombre de répétitions à effectuer pour chaque direction.
    * @param out_iNbOfMovement Nombre de directions de déplacement.
    * @return Un tableau d'entiers représentant les déplacements possibles pour un petit roque.
    */
    static int* getLittleRockMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    /**
    * Récupère les déplacements possibles pour un grand roque.
    *
    * @param out_iNbOfRepetitionToDo Nombre de répétitions à effectuer pour chaque direction.
    * @param out_iNbOfMovement Nombre de directions de déplacement.
    * @return Un tableau d'entiers représentant les déplacements possibles pour un grand roque.
    */
    static int* getBigRockMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    /**
    * Récupère les déplacements possibles pour un cavalier.
    *
    * @param out_iNbOfRepetitionToDo Nombre de répétitions à effectuer pour chaque direction.
    * @param out_iNbOfMovement Nombre de directions de déplacement.
    * @return Un tableau d'entiers représentant les déplacements possibles du cavalier.
    */
    static int* getKnightMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    /**
    * Retourne les déplacements possibles pour une tour.
    *
    * @param out_iNbOfRepetitionToDo Nombre de répétitions à effectuer pour chaque direction.
    * @param out_iNbOfMovement Nombre de directions de déplacement.
    * @return Un tableau d'entiers représentant les déplacements possibles de la tour.
    */
    static int* getRookMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    /**
    * Retourne les déplacements possibles pour un fou.
    *
    * @param out_iNbOfRepetitionToDo Nombre de répétitions à effectuer pour chaque direction.
    * @param out_iNbOfMovement Nombre de directions de déplacement.
    * @return Un tableau d'entiers représentant les déplacements possibles du fou.
    */
    static int* getBishopMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    /**
    * Retourne les déplacements possibles pour une reine.
    *
    * @param out_iNbOfRepetitionToDo Nombre de répétitions à effectuer pour chaque direction.
    * @param out_iNbOfMovement Nombre de directions de déplacement.
    * @return Un tableau d'entiers représentant les déplacements possibles de la reine.
    */
    static int* getQueenMoves(int& out_iNbOfRepetitionToDo, int& out_iNbOfMovement);
    /**
    * Retourne les déplacements possibles pour un pion.
    *
    * @param out_iNbOfRepetitionToDo Nombre de répétitions à effectuer pour chaque direction.
    * @param out_iNbOfMovement Nombre de directions de déplacement.
    * @param in_colPiece La couleur du pion (blanc ou noir).
    * @return Un tableau d'entiers représentant les déplacements possibles du pion.
    */
    static int* getPawnMoves(int &out_iNbOfRepetitionToDo, int& out_iNbOfMovement, Color in_colPiece);

    /**
    * Vérifie si la position donnée est valide (entre 0 et 63).
    *
    * @param position La position à vérifier.
    * @return True si la position est valide, sinon false.
    */
    static bool isValidPosition(int position);

    /**
    * Vérifie si la position suivante est valide pour un fou.
    *
    * @param in_iDirection La direction du déplacement.
    * @param in_iNextPosition La position suivante à vérifier.
    * @return True si la position suivante est valide, sinon false.
    */
    bool isBishopNextPositionValid(int in_iDirection, int in_iNextPosition);
    /**
    * Vérifie si la position suivante est valide pour une tour.
    *
    * @param in_iDirection La direction du déplacement.
    * @param in_iNextPosition La position suivante à vérifier.
    * @return True si la position suivante est valide, sinon false.
    */
    bool isRookNextPositionValid(int in_iDirection, int in_iNextPosition);
    /**
    * Vérifie si la position suivante est valide pour un cavalier.
    *
    * @param in_iDirection La direction du déplacement.
    * @param in_iInitialPosition La position initiale du cavalier.
    * @param in_iNextPosition La position suivante à vérifier.
    * @return True si la position suivante est valide, sinon false.
    */
    static bool isKnightNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition);
    /**
    * Vérifie si la position suivante est valide pour un pion.
    *
    * @param in_iDirection La direction du déplacement.
    * @param in_iInitialPosition La position initiale du pion.
    * @param in_iNextPosition La position suivante à vérifier.
    * @return True si la position suivante est valide, sinon false.
    */
    bool isPawnNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition);
    /**
    * Vérifie si la position suivante est valide pour un roque.
    *
    * @param in_iDirection La direction du roque.
    * @param in_iNextPosition La position suivante à vérifier.
    * @return True si la position suivante est valide, sinon false.
    */
    bool isRockNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition) const;

    /**
    * Vérifie si la position suivante est valide en fonction du type de la pièce.
    *
    * @param in_iDirection La direction du déplacement.
    * @param in_iInitialPosition La position initiale de la pièce.
    * @param in_iNextPosition La position suivante à vérifier.
    * @return True si la position suivante est valide, sinon false.
    */
    bool isNextPositionValid(int in_iDirection, int in_iInitialPosition, int in_iNextPosition);

    /**
    * Vérifie si la position suivante est valide. C'est à dire qui ne sort pas des limites du plateau
    *
    * @param in_iDirection La direction du déplacement.
    * @param in_iInitialPosition La position initiale de la pièce.
    * @return True si la position suivante est valide, sinon false.
    */
    static bool isNextPositionNotOutOfBoard(int in_iDirection, int in_iInitialPosition);

    /**
    * Vérifie si la pièce peut bouger dans une direction donnée (directions de bases)
    *
    * @param in_iDirection La direction du déplacement.
    * @return True si la pièce a le droit de bouger dans la direction donnée.
    */
    bool doesPieceMoveInDirection(int in_iDirection);


private:
    /**
    * Vérifie l'élément donnée est dans le tableau
    *
    * @param element L'élément à trouver dans le tableau
    * @param tableau Tableau dans lequel chercher l'élément
    * @param nbElement Nombres d'éléments dans le tableau
    * @return True si l'élément est dans le tableau, false sinon
    */
    bool elementEstDansTableau(int element, const int tableau[], int nbElement);
};

#endif //PIECE_H
