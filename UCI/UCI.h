//
// Created by thiba on 29/10/2024.
//

#ifndef UCI_H
#define UCI_H
#include <string>

#include "MainChessGame.h"


class UCI {
private:
    //Attributs de classes
    MainChessGame *m_mainChessGame;
    bool m_debugMode;


public:
    /**
    * @brief Initialise un moteur UCI pour la communication.
    *
    * Ce constructeur initialise une nouvelle instance de la classe `UCI`.
    * Par défaut, il initialise le mode de jeu en Joueur contre Bot (JVB) et
    * désactive le mode debug.
    */
    UCI();

    static std::string m_engineName;
    static std::string m_engineCode;
    static GameMode m_gameMode;

    /**
    * Gère la communication UCI avec l'interface d'échecs.
    *
    * Cette fonction lit les commandes de l'interface UCI en boucle et appelle les
    * fonctions correspondantes en fonction des commandes reçues. Elle s'arrête
    * lorsque la commande "quit" est reçue.
    */
    void uciCommunication();
    /**
    * Répond à la commande UCI "uci".
    *
    * Affiche les informations sur le moteur, y compris son nom et l'auteur, puis
    * signale que le moteur est prêt avec "uciok".
    */
    void inputUCI();
    /**
    * Répond à la commande "isready".
    *
    * Indique que le moteur est prêt en envoyant "readyok" à l'interface.
    */
    void inputIsReady();
    /**
    * Active ou désactive le mode debug en fonction de l'argument fourni.
    *
    * @param debugMode "on" pour activer le mode debug, "off" pour le désactiver.
    * Si activé, des informations de débogage sont affichées.
    */
    void inputDebug(const std::string &in_sInput);
    /**
    * @brief Initialise une nouvelle partie d'échecs.
    *
    * Cette fonction réinitialise l'instance de `MainChessGame` pour démarrer une
    * nouvelle partie.
    */
    void inputUCINewGame();
    /**
    * @brief Définit la position du plateau en fonction de l'entrée UCI.
    *
    * Cette fonction prend en charge la définition de la position initiale (avec
    * "startpos") ou une position spécifique en FEN (avec "fen"). Elle applique
    * ensuite une série de coups si fournie.
    *
    * @param in_sInput Commande de position, sous la forme de "position [startpos|fen] moves ..."
    */
    void inputPosition(std::string &in_sInput) const;
    /**
    * Déclenche le calcul du meilleur coup en fonction de la profondeur.
    *
    * @param in_sInput Commande "go" optionnellement suivie de "depth [n]", pour
    * spécifier la profondeur de recherche.
    */
    void inputGo(std::string &in_sInput);
    /**
    * Arrête la recherche du meilleur coup et retourne immédiatement un coup.
    */
    void inputStop();
    /**
    * Recherche et envoie le meilleur coup trouvé à l'interface.
    *
    * @param depth La profondeur maximale pour la recherche du coup. Si `-1`, utilise
    * une profondeur par défaut.
    */
    void findBestMove(int depth);
    /**
    * Gère la commande "register" pour enregistrer le moteur avec un nom ou un code.
    *
    * @param in_sInput La commande "register" suivie de "name" ou "code" et les
    * informations d'enregistrement. Affiche "registration success" ou "registration error"
    * selon le résultat.
    */
    void inputRegister(const std::string &in_sInput);
};



#endif //UCI_H
