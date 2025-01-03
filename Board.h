//
// Created by Peter on 02/10/2024.
//

#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Type.h"
#include <vector>
#include <cstdint>

class Board {
private:
    Piece* m_tabpiBoard[64]{};
    bool m_isWhiteKingChecked = false;
    bool m_isBlackKingChecked = false;

    bool m_bWhiteKingCanLittleRock = true;
    bool m_bWhiteKingCanBigRock = true;
    bool m_bBlackKingCanLittleRock = true;
    bool m_bBlackKingCanBigRock = true;

    int m_iBlackKingPosition = 60;
    int m_iWhiteKingPosition = 4;
    int m_ipositionEnPassant = -1;

    int m_iPreviousMoveInitialPosition = -1; // FOR DEBUG
    int m_iPreviousMoveTargetPosition = -1; // FOR DEBUG

    int m_itabIndexRockPrivilegeLostOnMove[4] = {-1, -1, -1, -1}; // Used for Zobrist

    uint64_t zobristHash;

    const int MVV_LVA[7][7] = {
        { 0,  0,  0,  0,  0,  0, 0 },      // Victim K, Attacker K, Q, R, B, N, P, None
        { 50, 51, 52, 53, 54, 55, 0 },    // Victim Q, Attacker K, Q, R, B, N, P, None
        { 40, 41, 42, 43, 44, 45, 0 },    // Victim R, Attacker K, Q, R, B, N, P, None
        { 30, 31, 32, 33, 34, 35, 0 },    // Victim B, Attacker K, Q, R, B, N, P, None
        { 20, 21, 22, 23, 24, 25, 0 },    // Victim N, Attacker K, Q, R, B, N, P, None
        { 10, 11, 12, 13, 14, 15, 0 },    // Victim P, Attacker K, Q, R, B, N, P, None
        { 0,  0,  0,  0,  0,  0, 0 }      // Victim None, Attacker K, Q, R, B, N, P, None
    };

    const int BlackPawnSquareTable[64] =
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    };

    const int WhitePawnSquareTable[64] =
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10,-20,-20, 10, 10,  5,
        5, -5,-10,  0,  0,-10, -5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5,  5, 10, 25, 25, 10,  5,  5,
        10, 10, 20, 30, 30, 20, 10, 10,
        50, 50, 50, 50, 50, 50, 50, 50,
        0,  0,  0,  0,  0,  0,  0,  0
    };

    const int BlackKnightSquareTable[64] =
    {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
    };

    const int WhiteKnightSquareTable[64] =
    {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
    };

    const int BlackBishopSquareTable[64] =
    {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20,
    };

    const int WhiteBishopSquareTable[64] =
    {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -20,-10,-10,-10,-10,-10,-10,-20,
    };

    const int BlackRookSquareTable[64] =
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0,
    };

    const int WhiteRookSquareTable[64] =
    {
        0,  0,  0,  5,  5,  0,  0,  0,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        5, 10, 10, 10, 10, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0,
    };

    const int BlackQueenSquareTable[64] =
    {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20,
    };

    const int WhiteQueenSquareTable[64] =
    {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -10,  5,  5,  5,  5,  5,  0,-10,
        0,  0,  5,  5,  5,  5,  0, -5,
        -5,  0,  5,  5,  5,  5,  0, -5,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20,
    };



public:
    //Initialisation and Setup Functions
    /**
    * @brief Constructeur de la classe `Board`.
    *
    * Ce constructeur initialise un objet `Board`, avec toutes les cases du plateau
    * vides et les droits de roque activés pour les deux rois. Les positions des rois
    * sont initialisées aux cases de départ standard (e1 pour le roi blanc et e8 pour le roi noir).
    */
    Board();
    ~Board();
    /**
    * @brief Initialise le plateau avec la configuration standard des pièces.
    *
    * Cette fonction place les pièces sur le plateau de jeu dans la configuration de départ standard
    * avec les pions blancs et noirs en rangées 2 et 7 respectivement, et les autres pièces sur les rangées 1 et 8.
    */
    void initializeBoard();
    /**
    * @brief Vide le plateau de toutes les pièces et réinitialise les paramètres du roi et les droits de roque.
    *
    * Cette fonction nettoie le plateau en supprimant toutes les pièces, en réinitialisant la position des rois
    * et en réactivant les droits de roque pour les deux rois. Elle remet également les indicateurs de mise en échec à `false`.
    */
    void clearBoard();
    /**
    * @brief Configure le plateau à partir d'une chaîne FEN.
    *
    * Cette fonction analyse la notation FEN fournie et place les pièces correspondantes sur le plateau.
    * Elle met à jour les positions des rois et vérifie si chaque roi est en échec une fois le plateau configuré.
    *
    * @param fen Une chaîne FEN décrivant la position des pièces sur le plateau.
    */
    void setupFromFEN(const std::string& fen);
    /**
    * @brief Définit les droits de roque en fonction de la notation FEN.
    *
    * Cette fonction analyse une chaîne partielle de la notation FEN pour définir les droits
    * de roque pour le roi blanc et noir, en fonction des lettres présentes.
    *
    * @param castling La sous-chaîne de notation FEN indiquant les droits de roque (ex. "KQkq").
    */
    void setCastlingRightsForFenNotation(const std::string& castling);

    //Piece Placement and Movement Functions
    /**
    * Place une pièce sur l'échiquier en utilisant la notation échiquéenne.
    *
    * @param move Une chaîne représentant la position cible en notation échiquéenne (par exemple, "e4").
    * @param in_pPiece Pointeur vers la pièce à placer sur l'échiquier.
    * @return Vrai si la pièce est placée avec succès, faux si la position est invalide.
    */
    bool placePiece(const std::string& move, Piece* in_pPiece);
    /**
    * Place une pièce sur l'échiquier à une ligne et une colonne spécifiques.
    *
    * @param in_iRow L'index de la ligne (0-7).
    * @param in_iCol L'index de la colonne (0-7).
    * @param in_pPiece Pointeur vers la pièce à placer sur l'échiquier.
    * @return Vrai si la pièce est placée avec succès, faux si la ligne ou la colonne est invalide.
    */
    bool placePiece(int in_iRow, int in_iCol, Piece* in_pPiece);
    /**
    * Place une pièce sur l'échiquier à une position spécifique.
    *
    * @param in_iPositionPiece L'index de la position sur l'échiquier (0-63).
    * @param in_pPiece Pointeur vers la pièce à placer sur l'échiquier.
    * @return Vrai si la pièce est placée avec succès, faux si la position est invalide.
    */
    bool placePiece(int in_iPositionPiece, Piece* in_pPiece);
    /**
    * Déplace une pièce d'une ligne et d'une colonne de départ vers une ligne et une colonne de destination.
    *
    * @param in_iStartRow L'index de la ligne de départ (0-7).
    * @param in_iStartCol L'index de la colonne de départ (0-7).
    * @param in_iEndRow L'index de la ligne de destination (0-7).
    * @param in_iEndCol L'index de la colonne de destination (0-7).
    * @param in_colPlayer La couleur du joueur qui effectue le déplacement.
    * @return Vrai si le déplacement est réussi, faux si le déplacement est invalide.
    */
    bool movePiece(int in_iStartRow, int in_iStartCol, int in_iEndRow, int in_iEndCol, Color in_colPlayer = Color::WHITE);
    /**
    * Déplace une pièce en utilisant la notation échiquéenne et gère la promotion des pions.
    *
    * @param move Une chaîne représentant le déplacement en notation échiquéenne (par exemple, "e2e4").
    * @param in_colPlayer La couleur du joueur qui effectue le déplacement.
    * @param promotionType Le type de pièce pour la promotion, si applicable.
    * @return Vrai si le déplacement est réussi, faux si le déplacement est invalide.
    */
    bool movePiece(const std::string& move, Color in_colPlayer = Color::WHITE, TypePieces promotionType = TypePieces::QUEEN);
    /**
    * Déplace une pièce avec gestion supplémentaire des captures en passant et de la promotion.
    *
    * @param in_colPlayer La couleur du joueur qui effectue le déplacement.
    * @param move Le déplacement en notation échiquéenne (par exemple, "e2e4").
    * @param piece Pointeur pour stocker toute pièce capturée, si applicable.
    * @param promotionType Le type de pièce pour la promotion, si applicable.
    * @param enPassantPos Pointeur pour stocker la position en passant, si applicable.
    * @return Vrai si le déplacement est réussi, faux si le déplacement est invalide.
    */
    bool movePiece(Color in_colPlayer, const std::string& move, Piece** piece = nullptr,TypePieces promotionType = TypePieces::NONE, int* enPassantPos = nullptr);
    /**
    * Déplace une pièce d'une position de départ à une position de destination et gère les règles comme la prise en passant, le roque et la promotion.
    *
    * @param in_iStartPosition L'index de la position de départ sur l'échiquier (0-63).
    * @param in_iEndPosition L'index de la position de destination sur l'échiquier (0-63).
    * @param in_colPlayer La couleur du joueur qui effectue le déplacement.
    * @param piece Pointeur pour stocker toute pièce capturée, si applicable.
    * @param promotionType Le type de pièce pour la promotion, si applicable.
    * @param enPassantPos Pointeur pour stocker la position en passant, si applicable.
    * @return Vrai si le déplacement est réussi, faux si le déplacement est invalide.
    */
    bool movePiece(int in_iStartPosition, int in_iEndPosition, Color in_colPlayer = Color::WHITE, Piece** piece = nullptr,TypePieces promotionType = TypePieces::NONE, int* enPassantPos = nullptr);

    //Checking and Validation Functions
    /**
    * Vérifie si le roi blanc est en échec.
    *
    * @return Vrai si le roi blanc est en échec, faux sinon.
    */
    bool isWhiteKingCheck() const;
    /**
    * Vérifie si le roi noir est en échec.
    *
    * @return Vrai si le roi noir est en échec, faux sinon.
    */
    bool isBlackKingCheck() const;
    /**
    * Vérifie si un déplacement est possible entre deux positions.
    *
    * @param in_iStartPosition La position de départ (0-63).
    * @param in_iTargetPosition La position cible (0-63).
    * @return Vrai si le déplacement est possible, faux sinon.
    */
    bool isMovementPossible(int in_iStartPosition, int in_iTargetPosition);
    //@TODO Sert à rien mais est dans les tests
    bool isCheckmated(int in_iStartRow, int in_iStartCol, Color in_colPlayer);
    /**
    * Vérifie si le roi d'une couleur spécifique est attaqué après un déplacement.
    *
    * @param in_kingColor La couleur du roi (Color::WHITE ou Color::BLACK).
    * @param in_attackerColor La couleur de l'attaquant (Color::WHITE ou Color::BLACK).
    * @return Vrai si le roi est attaqué après le déplacement, faux sinon.
    */
    bool isKingAttackedAfterMove(Color in_kingColor, Color in_attackerColor) const;
    /**
    * Vérifie si le roi d'une couleur est en échec.
    *
    * @param in_kingColor La couleur du roi (Color::WHITE ou Color::BLACK).
    * @return Vrai si le roi est en échec, faux sinon.
    */
    bool isKingInCheck(Color in_kingColor) const;
    /**
    * Définit si un roi est en échec ou non.
    *
    * @param in_kingColor La couleur du roi (Color::WHITE ou Color::BLACK).
    * @param in_bPutKingInCheck Vrai pour mettre le roi en échec, faux pour le retirer de l'échec.
    */
    void putOrRemoveKingInCheck(Color in_kingColor, bool in_bPutKingInCheck);
    /**
    * Vérifie si un mouvement est un mouvement de promotion de pion.
    *
    * @param start La position de départ du pion (0-63).
    * @param end La position d'arrivée (0-63).
    * @param color La couleur du joueur (Color::WHITE ou Color::BLACK).
    * @return Vrai si le mouvement est un mouvement de promotion, faux sinon.
    */
    bool isPromotionMove(int start, int end, Color color);
    /**
    * Vérifie si le roi peut effectuer un roque dans une direction donnée.
    *
    * @param in_colKing La couleur du roi (Color::WHITE ou Color::BLACK).
    * @param in_iDirectionForRock La direction du roque (1 pour le petit roque, -1 pour le grand roque).
    * @return Vrai si le roi peut effectuer le roque, faux sinon.
    */
    bool doesKingCanRock(Color in_colKing, int in_iDirectionForRock) const;
    /**
    * Vérifie si le roi peut effectuer un petit roque.
    *
    * @param in_kingColor La couleur du roi (Color::WHITE ou Color::BLACK).
    * @return Vrai si le roi blanc peut effectuer un petit roque, faux sinon.
    */
    bool kingCanLittleRock(Color in_kingColor) const;
    /**
    * Vérifie si le roi peut effectuer un grand roque.
    *
    * @param in_kingColor La couleur du roi (Color::WHITE ou Color::BLACK).
    * @return Vrai si le roi peut effectuer un grand roque, faux sinon.
    */
    bool kingCanBigRock(Color in_kingColor) const;

    //Attack and Threat Detection Functions
    /**
    * Vérifie si une case est attaquée par une couleur spécifique.
    *
    * @param in_iPosition La position de la case à vérifier (index de la case sur l'échiquier).
    * @param in_colorToFindAttack La couleur des pièces qui doivent attaquer la case.
    * @param in_vectPositionPieceFound Un vecteur qui contient les positions des pièces trouvées qui attaquent la case.
    * @return true Si la case est attaquée par une pièce de la couleur spécifiée, false sinon.
    */
    bool isCaseAttackedByColor(int in_iPosition, Color in_colorToFindAttack, std::vector<int>& in_vectPositionPieceFound) const;
    /**
    * Vérifie si une case est attaquée par n'importe quelle couleur.
    *
    * @param in_iPosition La position de la case à vérifier (index de la case sur l'échiquier).
    * @param in_vectPositionPieceFound Un vecteur qui contiendra les positions des pièces attaquantes trouvées.
    * @return true Si la case est attaquée par une pièce de n'importe quelle couleur, false sinon.
    */
    bool isCaseAttackedByAnyColor(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    /**
    * Trouve la première pièce dans une direction donnée qui attaque une case initiale.
    *
    * @param in_iPosition La position de départ de la recherche (index de la case sur l'échiquier).
    * @param in_iDirection La direction de recherche, indiquée par un entier représentant un déplacement sur l'échiquier.
    * @param in_iNbOfRepetition Le nombre de répétitions de mouvements à faire dans cette direction.
    * @param in_iPositionPieceFound La position où la première pièce attaquante a été trouvée.
    * @return Un pointeur vers la pièce qui attaque la case initiale, ou nullptr si aucune pièce n'est trouvée.
    */
    Piece* findFirstPieceOnDirectionThatAttacksInitialPosition(int in_iPosition,int in_iDirection, int in_iNbOfRepetition, int& in_iPositionPieceFound) const;
    /**
    * Trouve la première pièce dans une direction donnée.
    *
    * @param in_iPosition La position de départ de la recherche (index de la case sur l'échiquier).
    * @param in_iDirection La direction de recherche, indiquée par un entier représentant un déplacement sur l'échiquier.
    * @param in_iNbOfRepetition Le nombre de répétitions de mouvements à faire dans cette direction.
    * @param in_iPositionPieceFound La position où la première pièce a été trouvée.
    * @return Un pointeur vers la pièce trouvée, ou nullptr si aucune pièce n'est trouvée.
    */
    Piece* findFirstPieceOnDirection(int in_iPosition,int in_iDirection, int in_iNbOfRepetition, int& in_iPositionPieceFound) const;
    /**
    * Recherche les pièces attaquantes dans toutes les directions possibles pour une tour.
    *
    * @param in_iPosition La position de la case à vérifier (index de la case sur l'échiquier).
    * @param in_vectPositionPieceFound Un vecteur qui contient les positions des pièces attaquantes trouvées.
    */
    void findFirstPiecesOnEachRookMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    /**
    * Recherche les pièces attaquantes dans toutes les directions possibles pour un fou.
    *
    * @param in_iPosition La position de la case à vérifier (index de la case sur l'échiquier).
    * @param in_vectPositionPieceFound Un vecteur qui contient les positions des pièces attaquantes trouvées.
    */
    void findFirstPiecesOnEachBishopMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;
    /**
    * Recherche les pièces attaquantes pour un cavalier à partir d'une position donnée.
    *
    * @param in_iPosition La position de la case à vérifier (index de la case sur l'échiquier).
    * @param in_vectPositionPieceFound Un vecteur qui contient les positions des pièces attaquantes trouvées.
    */
    void findFirstPiecesOnEachKnightMovementsThatAttacksInitialPosition(int in_iPosition, std::vector<int>& in_vectPositionPieceFound) const;

    //Piece Movement Options and Calculations
    /**
    * Récupère les mouvements possibles pour une pièce à partir d'une position donnée.
    *
    * @param in_iPositionToSeeMoves La position de la pièce dont on souhaite déterminer les mouvements possibles.
    * @param in_vectPossibleMoves Un vecteur qui sera rempli avec les positions des cases où la pièce peut se déplacer.
    *                             Ce vecteur est modifié directement dans la fonction.
    */
    void possibleMovesForPiece(int in_iPositionToSeeMoves, std::vector<int>& in_vectPossibleMoves);
    /**
    * Récupère les mouvements possibles d'une pièce dans une direction spécifique avec un nombre donné de répétitions.
    *
    * @param in_iPositionToFindMovement La position de départ de la pièce.
    * @param in_iDirectionMovement La direction dans laquelle la pièce doit se déplacer.
    * @param in_iNbOfRepetition Le nombre de répétitions des déplacements dans la direction donnée.
    * @param in_vectPositionPossible Un vecteur qui sera rempli avec les positions possibles obtenues en répétant le mouvement.
    */
    void getPieceMovementsPossible(int in_iPositionToFindMovement, int in_iDirectionMovement, int in_iNbOfRepetition, std::vector<int>& in_vectPositionPossible);
    /**
    * Récupère tous les mouvements possibles pour une pièce à partir de sa position, en explorant toutes les directions.
    *
    * @param in_iPositionToFindMovement La position de départ de la pièce.
    * @param out_vectDirectionToFill Un vecteur qui sera rempli avec les directions possibles pour la pièce à partir de sa position.
    */
    void getAllPossibleMovementsForAPiece(int in_iPositionToFindMovement, std::vector<int>& out_vectDirectionToFill);
    /**
    * Vérifie et ajoute un mouvement valide à une liste de mouvements possibles.
    *
    * @param in_iNextPosition La position vers laquelle la pièce est censée se déplacer.
    * @param in_pPieceToMove Un pointeur vers la pièce qui doit être déplacée.
    * @param in_vectMoveToFill Un vecteur qui sera rempli avec le mouvement valide si la position est valide.
    * @return `true` si le mouvement est valide et a été ajouté au vecteur, `false` sinon.
    */
    bool putNextMoveIfValid(int in_iNextPosition, Piece* in_pPieceToMove, std::vector<int>& in_vectMoveToFill);
    /**
    * Génère la liste de tous les mouvements possibles pour toutes les pièces d'une couleur donnée.
    *
    * @param in_colColor La couleur des pièces pour lesquelles les mouvements doivent être calculés (blanc ou noir).
    * @return Un vecteur de paires représentant les mouvements possibles pour chaque pièce de la couleur spécifiée,
    *         où chaque paire contient la position de départ et la position d'arrivée.
    */
    std::vector<std::pair<int, int>> listOfPossibleMoves(Color in_colColor);
    /**
    * Annule un mouvement effectué, en revenant à la position initiale et en rétablissant les pièces capturées.
    *
    * @param in_iStartPosition La position de départ de la pièce déplacée.
    * @param in_iEndPosition La position d'arrivée de la pièce déplacée.
    * @param capturedPiece Un pointeur vers la pièce capturée (si applicable) qui sera restaurée.
    * @param promotion Indique si une promotion de pion a eu lieu, dans ce cas la pièce sera replacée avant promotion.
    * @param enPassantPos La position d'une capture en passant, si applicable. Si aucune capture en passant n'a eu lieu,
    *                     la valeur par défaut de -1 sera utilisée.
    * @return `true` si le mouvement a été annulé avec succès, `false` en cas d'erreur.
    */
    bool undoMove(int in_iStartPosition, int in_iEndPosition, Piece* capturedPiece,bool promotion=false,int enPassantPos=-1, int in_itabCastlingRights[4] = nullptr, bool in_bIsWhiteKingChecked = false, bool in_bIsBlackKingChecked = false, int in_iWhiteKingPosition = -1, int in_iBlackKingPosition = -1);

    //Evaluation and Heuristic Functions
    /**
    * Évalue la position actuelle du plateau pour un joueur donné.
    *
    * @param in_colPlayer La couleur du joueur dont l'évaluation est demandée (blanc ou noir).
    * @return Un score entier représentant l'évaluation de la position pour le joueur donné.
    */
    bool isInDanger(Piece* piece, int position);
    bool isAttackedByKnight(int position, Color opponentColor);
    bool isAttackedByKing(int position, Color opponentColor);
    bool isAttackedByRookOrQueen(int position, Color opponentColor);
    bool isAttackedByBishopOrQueen(int position, Color opponentColor);
    bool isAttackedByPawn(int position, Color opponentColor);
    int evaluateSimple(Color in_colPlayer);
    int evaluateTest(Color in_colPlayer);
    int evaluate(Color in_colPlayer);
    int GetSquareValue(Piece* piece, int pos);
    bool isCenterSquare(int index);
    bool isInitialPosition(Piece* piece, int index);
    int countProtections(Piece* piece, int pos);
    int countThreats(Piece* piece, int pos);
    bool isMultipleAttack(Piece* piece, int pos);
    bool isSingleAttack(Piece* piece, int pos);
    int getOpponentKingPosition(Color playerColor) ;
    bool isTowardsOpponentKing(Piece* piece, int pos, int opponentKingPosition);
    int evaluateMobility(Color in_colPlayer) const;
    int getPieceMobility(Piece* piece, int position) const;
    int evaluatePawnMobility(Piece* piece, int position) const;
    int evaluateKnightMobility(Piece* piece, int position) const;
    int evaluateBishopMobility(Piece* piece, int position) const;
    int evaluateRookMobility(Piece* piece, int position) const;
    int evaluateQueenMobility(Piece* piece, int position) const;
    int evaluateKingMobility(Piece* piece, int position) const;
    /**
    * Évalue un mouvement spécifique (capture, structure des pions, sécurité du roi).
    *
    * @param move Le mouvement à évaluer, représenté par une paire de positions (origine, destination).
    * @param color La couleur du joueur dont le mouvement est évalué.
    * @return Un score représentant la valeur du mouvement pour le joueur donné.
    */
    int evaluateMove(const std::pair<int, int>& move, Color color);
    /**
    * Évalue la structure des pions pour une position donnée.
    *
    * Cette fonction évalue la structure des pions pour une position donnée sur l'échiquier, en tenant compte des
    * éléments suivants :
    * - Pions isolés : les pions qui n'ont pas de voisins dans les colonnes adjacentes, ce qui les rend plus vulnérables.
    * - Pions doublés : les pions qui se trouvent sur la même colonne, ce qui les rend plus faibles dans certaines situations.
    * - Pions avancés : les pions qui se trouvent loin de leur ligne de départ sont mieux évalués s'ils sont avancés.
    * - Protection diagonale : les pions protégés par d'autres pièces en diagonale, comme un fou ou une autre pièce.
    * - Colonnes ouvertes : les colonnes sans pions adverses sont favorisées pour le contrôle de l'échiquier.
    *
    * @param position La position du pion à évaluer.
    * @param color La couleur du joueur dont le pion est évalué.
    * @return Un score qui évalue la qualité de la structure du pion.
    */
    int evaluatePawnStructure(int position, Color color) const;
    /**
    * Évalue la sécurité du roi pour un joueur donné.
    *
    * Cette fonction évalue la sécurité du roi en vérifiant les cases adjacentes à la position du roi. Si une pièce
    * ennemie se trouve à proximité immédiate, un score négatif est appliqué pour signaler une menace. Si le roi est
    * protégé par des pièces alliées, un score positif est ajouté.
    *
    * @param color La couleur du joueur dont la sécurité du roi est évaluée.
    * @return Un score représentant la sécurité du roi pour la couleur donnée.
    */
    int evaluateKingSafety(Color color) const;
    /**
    * Récupère la valeur associée à un type de pièce.
    *
    * @param type Le type de la pièce dont on souhaite obtenir la valeur.
    * @return La valeur associée à la pièce donnée.
    */
    int getPieceValue(TypePieces type);

    //Promotion and Castling Functions
    /**
    * Promouvoit un pion en une pièce de rang supérieur (Reine, Tour, Fou, Cavalier).
    * Si aucun type de promotion n'est spécifié, l'utilisateur est invité à choisir parmi les options disponibles.
    *
    * @param in_colPlayer La couleur du joueur qui effectue la promotion.
    * @param ppPiece Un pointeur vers le pion à promouvoir. Il sera mis à jour avec la nouvelle pièce.
    * @param promotionType Le type de promotion de pièce choisi. Si aucune promotion n'est spécifiée, l'utilisateur est invité à choisir.
    */
    void promotePawn(Color in_colPlayer, Piece** ppPiece,TypePieces promotionType = TypePieces::NONE);
    /**
    * Retire la possibilité de roque pour un joueur en fonction de la colonne de la tour concernée.
    *
    * @param in_colPlayer La couleur du joueur pour lequel la possibilité de roque doit être mise à jour.
    * @param i_columnRook La colonne de la tour qui a été déplacée. La tour de la colonne 0 est la tour gauche, celle de la colonne 7 est la tour droite.
    */
    void removeRockPossibility(Color in_colPlayer, int i_columnRook);

    //Utility and Conversion Functions
    /**
    * Vérifie si une position donnée est valide sur le plateau (entre 0 et 63 inclus).
    *
    * @param in_iPosition La position à vérifier, exprimée sous forme d'un entier.
    * @return `true` si la position est valide, `false` sinon.
    */
    static bool isValidPosition(int in_iPosition);
    /**
    * Récupère la position en passant actuellement active.
    *
    * @return La position en passant, exprimée sous forme d'un entier. Si aucune position en passant n'est active, renvoie une valeur par défaut (-1).
    */
    int getEnPassantPosition() const;
    /**
    * Définit la position en passant actuelle.
    *
    * @param pos La position à définir pour le coup en passant.
    */
    void setEnPassantPosition(int pos);
    /**
    * Récupère un index associé à une pièce donnée, en fonction de son type et de sa couleur.
    * La couleur blanche ajoute 1 au résultat pour différencier les pièces blanches et noires.
    *
    * @param type Le type de la pièce (Pion, Tour, Cavalier, Fou, Reine, Roi).
    * @param color La couleur de la pièce (Blanc ou Noir).
    * @return L'index correspondant à la pièce donnée, avec les valeurs de 0 à 11.
    */
    int getIndexByPiece(TypePieces type, Color color) const;
    /**
    * Récupère l'état actuel du plateau sous forme de vecteur d'entiers.
    * Chaque case est représentée par un entier correspondant à l'index de la pièce sur cette case,
    * ou 0 si la case est vide.
    *
    * @return Un vecteur d'entiers de taille 64 représentant l'état du plateau.
    */
    static int getIndexForPiece(TypePieces type);
    std::vector<int> getBoardStateAsVector() const;

    /**
     * Récupère l'état actuel du plateau sous forme de vecteur d'entiers.
     * indice dans le tableau retourné :
     *      - 0 : white little rock (king side) (valeur 0)
     *      - 1 : white big rock (queen side) (valeur 1)
     *      - 2 : black little rock (king side) (valeur 2)
     *      - 3 : black big rock (queen side) (valeur 3)
     * @return Un vecteur d'entiers de taille 4 représentant l'état des droits de roque.
     */
    std::vector<int> getCastlingStateAsVector() const;

    void getCastlingStateAsTableau(int out_itabCastlingRights[4]) const;

    /**
     * Les indices retournés vont de 0 à 15.    0-7 : EnPassant noir en lisant de gauche à droite ||    8-15 : EnPassant blanc en lisant de gauche à droite
     * @return L'indice de la valeur en passant dans le tableau Zobrits
     */
    int getEnPassantState() const;
    
    /**
    * Convertit une position sur le plateau de notation alphanumérique (par exemple 'a1', 'h8') en une position interne.
    * La colonne est exprimée par une lettre entre 'a' et 'h', et la ligne par un chiffre entre '1' et '8'.
    *
    * @param col La colonne sous forme de caractère ('a' à 'h').
    * @param row La ligne sous forme de caractère ('1' à '8').
    * @return La position correspondante sur le plateau, exprimée sous forme d'un entier entre 0 et 63.
    */
    static int convertToPosition(char col, char row);
    /**
    * Convertit un mouvement de notation alphanumérique (par exemple "e2e4") en deux positions internes.
    *
    * @param move La chaîne de caractères représentant le mouvement (par exemple "e2e4").
    * @param startPos La position de départ du mouvement, qui sera mise à jour.
    * @param endPos La position d'arrivée du mouvement, qui sera mise à jour.
    */
    static void convertMoveToPositions(const std::string& move, int& startPos, int& endPos);

    //Piece Retrieval Functions
    /**
    * Récupère la pièce située à une position donnée sur le plateau.
    * Si la position n'est pas valide, la fonction retourne un pointeur nul.
    *
    * @param in_iPositionPiece La position sur le plateau (entre 0 et 63).
    * @return Un pointeur vers la pièce à la position donnée, ou `nullptr` si la position est vide ou invalide.
    */
    Piece* getPieceAt(int  in_iPositionPiece) const;
    /**
    * Récupère la pièce située à une position donnée, spécifiée en notation alphanumérique (par exemple "e2").
    * La position est convertie en une position interne avant de récupérer la pièce.
    *
    * @param in_sPosition La position sous forme de chaîne alphanumérique (exemple : "e2").
    * @return Un pointeur vers la pièce à la position donnée, ou `nullptr` si la case est vide.
    */
    Piece* getPieceAt(const std::string& in_sPosition) const;
    /**
    * Récupère la pièce située à une position spécifiée par ses coordonnées de ligne et de colonne.
    * Les coordonnées sont converties en une position interne avant de récupérer la pièce.
    *
    * @param in_iRow La ligne de la position (de 0 à 7).
    * @param in_iColumn La colonne de la position (de 0 à 7).
    * @return Un pointeur vers la pièce à la position donnée, ou `nullptr` si la case est vide.
    */
    Piece* getPieceAt(int in_iRow, int in_iColumn) const;

    //Game Status and Check/End Conditions
    /**
    * Récupère la position actuelle du roi d'une couleur donnée.
    *
    * @param in_kingColor La couleur du roi dont on veut connaître la position (blanc ou noir).
    * @return La position du roi sur le plateau, ou -1 si la couleur du roi est invalide.
    */
    int getKingPosition(Color in_kingColor) const;
    /**
    * Vérifie si la partie est terminée et détermine le gagnant (le cas échéant).
    * Si la partie est terminée, cette fonction met à jour la variable `out_colWinner` avec la couleur du gagnant.
    * Si la partie est terminée par un match nul, `out_colWinner` est mis à `Color::NONE`.
    *
    * @param colCurrent_player La couleur du joueur dont c'est le tour.
    * @param out_colWinner La couleur du gagnant de la partie, ou `Color::NONE` si la partie est nulle.
    * @return `true` si la partie est terminée, `false` sinon.
    */
    bool isGameOver(Color colCurrent_player, Color& out_colWinner);
    /**
    * Affiche l'état actuel du plateau dans le terminal.
    * Les pièces sont affichées sous forme de caractères, avec les pièces noires en majuscule
    * et les pièces blanches en minuscule. Les cases vides sont représentées par un point (".").
    */
    void displayBoard() const;
    void getCastlingRightsLostByMoving(int out_itabCastlingRightsLost[4]) const;
    std::string getBoardAsString() const;
    uint64_t getZobristHash() const { return zobristHash; }
    void setZobristHash(uint64_t hash) { zobristHash = hash; }
    void listOfPossibleMoves(Color in_colPlayer, std::vector<std::pair<int, int>>& out_moves);
    void listOfPossibleMoves(Color in_colPlayer, std::pair<int, int> out_moves[128], int& out_moveCount);
    void getAllPiecesEatableByAColor(Color in_playerColor, std::pair<int, int> out_tabMovesToEatPieces[], int &out_moveCount);

    /**
     * 
     * @return true s'il y a un pion enemie à côté du pion qui a fait l'enPassant (Si les blancs font e2 e4, ça va regarder en d4 et f4 s'il y a un pion noir)
     */
    bool isThereEnemyPawnNextToEnPassantPawn() const;
    void getEnemyPawnNextToEnPassantPawn(int out_iPositionPawnNextToEnPassant[2]) const;

    int getPreviousMoveInitialPosition() const {
        return m_iPreviousMoveInitialPosition;
    }

    void setPreviousMoveInitialPosition(int m_i_previous_move_initial_position) {
        m_iPreviousMoveInitialPosition = m_i_previous_move_initial_position;
    }

    int getPreviousMoveTargetPosition() const {
        return m_iPreviousMoveTargetPosition;
    }

    void setPreviousMoveTargetPosition(int m_i_previous_move_target_position) {
        m_iPreviousMoveTargetPosition = m_i_previous_move_target_position;
    }
};

#endif //BOARD_H
