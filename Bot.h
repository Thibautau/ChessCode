//
// Created by Peter on 02/10/2024.
//

#ifndef BOT_H
#define BOT_H

#include "Type.h"
#include "Board.h"
#include "Player.h"
#include <unordered_map>
#include <cstdint>
#include "LogFile.h"
#include "OpeningBook/OpeningBook.h"


const int EXACT = 0;
const int ALPHA_CUT = -1;
const int BETA_CUT = 1;
const int UPPERBOUND = 1;
const int LOWERBOUND = -1;

struct TranspositionTableEntry {
    int depth;
    int score;
    int bestMoveIndex;
    int flag; // 0: Exact, -1: Alpha cut, 1: Beta cut
};

class Bot : public Player {
private:
    Color m_color;
    std::unordered_map<uint64_t, TranspositionTableEntry> transpositionTable;
    //LogInFile* m_logFile;
    LogFile* m_logFile;
    static constexpr char PROMOTION_TYPES[4] = {'q', 'n', 'b', 'r'};
    static constexpr char NO_PROMOTION[1] = {'\0'};
    OpeningBook* m_openingBook;
    int m_max_depth_Quiescence = 6;
    int m_max_depth = 4;
    int m_diff_between_depth = 2;



public:
    static int nodeCount;
    //Si on a déjà parcouru un node alors on n'incrémente pas le compteur
    static int uniqueNodeIterated;
    Bot(Color color);
    /**
    * Fonction qui fait jouer le bot en choisissant un mouvement sur l'échiquier.
    *
    * Cette fonction appelle la méthode `choisir_meilleur_coup` pour choisir le meilleur coup,
    * puis affecte les cases de départ et d'arrivée aux paramètres `start` et `end`.
    *
    * @param board L'échiquier sur lequel le Bot joue.
    * @param start La position de départ du coup choisi par le Bot.
    * @param end La position d'arrivée du coup choisi par le Bot.
    */
    void play(Board& board, int& start, int& end) override;
    /**
    * Fonction qui fait jouer le bot en choisissant un mouvement avec une profondeur spécifiée (utilisé par l'UCI).
    *
    * Cette fonction prend en compte une profondeur de recherche spécifique pour la minimisation
    * ou maximisation de la recherche, ainsi que la gestion des promotions de pièces.
    *
    * @param board L'échiquier sur lequel le Bot joue.
    * @param start La position de départ du coup choisi par le Bot.
    * @param end La position d'arrivée du coup choisi par le Bot.
    * @param depth La profondeur à laquelle le Bot doit jouer.
    * @param promotion La promotion de la pièce si le coup est une promotion (si applicable).
    */
    void playWithDepth(Board &board, int &start, int &end, int depth, char &promotion);
    /**
    * Retourne la couleur du joueur contrôlé par le bot.
    *
    * @return La couleur du joueur (Blanc ou Noir).
    */
    Color getPlayerColor() const override;
    void setPlayerColor(Color color) override;
    /**
    * Choisit le meilleur coup en fonction d'une profondeur maximale.
    *
    * Cette fonction explore les coups possibles, évalue leur score en utilisant un moteur minimax
    * et sélectionne celui avec le meilleur score. La fonction prend également en compte les promotions.
    *
    * @param board L'échiquier sur lequel le Bot joue.
    * @param profondeur_max La profondeur maximale à utiliser dans l'algorithme minimax.
    * @param meilleurCoup La paire (start, end) représentant le meilleur coup choisi.
    * @param bestPromotion (optionnel) permet de récupérer la promotion si besoin.
    */
    void choisir_meilleur_coup(Board &board, int profondeur_max, std::pair<int, int> &meilleurCoup, char *bestPromotion=nullptr);
    /**
    * Fonction minimax pour évaluer le meilleur coup en fonction de la profondeur et des scores.
    *
    * Cette fonction implémente l'algorithme Minimax avec élagage alpha-bêta pour maximiser ou minimiser le score
    * du bot en fonction de la couleur du joueur et des coups possibles.
    *
    * @param board L'échiquier à évaluer.
    * @param profondeur La profondeur de recherche.
    * @param estMaximisant Indique si c'est le tour du bot (maximisant) ou de l'adversaire (minimisant).
    * @param alpha La borne inférieure pour l'élagage alpha-bêta.
    * @param beta La borne supérieure pour l'élagage alpha-bêta.
    * @param bestPromotion La meilleure promotion choisie (si applicable).
    *
    * @return Le score évalué pour le meilleur coup.
    */
    int minimax(Board &board, int profondeur, bool estMaximisant, int alpha, int beta, char &bestPromotion);
    /**
    * Évalue un mouvement en utilisant l'algorithme Minimax, en prenant en compte les promotions.
    *
    * Cette fonction effectue un mouvement temporaire sur l'échiquier, évalue le score du coup en utilisant Minimax,
    * puis annule le mouvement pour revenir à la position précédente. Elle gère également les promotions et captures.
    *
    * @param board L'échiquier sur lequel le coup doit être joué.
    * @param profondeur La profondeur de recherche pour l'évaluation Minimax.
    * @param estMaximisant Indique si c'est le tour du bot (maximisant) ou de l'adversaire (minimisant).
    * @param alpha La borne inférieure pour l'élagage alpha-bêta.
    * @param beta La borne supérieure pour l'élagage alpha-bêta.
    * @param move Le coup à évaluer, représenté par une paire de positions (start, end).
    * @param currentColor La couleur du joueur dont c'est le tour.
    * @param promotion La promotion de la pièce si le coup est une promotion (si applicable).
    *
    * @return Le score du coup évalué.
    */
    int Quiescence(Board &board, int depth, int alpha, int beta, bool estMaximisant,char &bestPromotion);
    int evaluateMoveWithMinimax(Board& board, int profondeur, bool estMaximisant, int alpha, int beta, const std::pair<int, int>& move, Color currentColor,char& promotion);
    int alphaBetaWithMemory(Board& board, int depth, int alpha, int beta, bool estMaximisant, char &bestPromotion);
    int alphaBetaWithMemoryv2(Board& board, int depth, int alpha, int beta, bool estMaximisant, char &bestPromotion);
    int alphaBetaBasic(Board& board, int depth, int alpha, int beta, bool estMaximisant, char &bestPromotion);
    int minimax(Board& board, int profondeur, bool estMaximisant, int alpha, int beta);
    void clearFile(const std::string& filename);
    int evaluateMoveWithMinimaxv2(Board& board, int profondeur, bool estMaximisant, int alpha, int beta, const std::pair<int, int>& move, Color currentColor, char& promotion);
    void choisir_meilleur_coupv2(Board& board, int profondeur_max, std::pair<int, int>& meilleurCoup, char* bestPromotion=nullptr);
    static void calculateZobristHashForMove(Board& board, const std::pair<int, int>& move, Color currentColor, char promotionForMove, bool isPromotion, uint64_t& zobristHash, Piece* capturedPiece=nullptr);
};

#endif //BOT_H
