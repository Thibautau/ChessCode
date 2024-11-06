// UCI.cpp
// Classe UCI (Universal Chess Interface) pour interagir avec les moteurs d'échecs en utilisant le protocole UCI

#include "UCI.h"

#include <iostream>
#include <sstream>

std::string UCI::m_engineName = "MonEngine v1";
std::string UCI::m_engineCode = "";
GameMode UCI::m_gameMode = GameMode::JVB;

/**
 * @brief Initialise un moteur UCI pour la communication.
 *
 * Ce constructeur initialise une nouvelle instance de la classe `UCI`.
 * Par défaut, il initialise le mode de jeu en Joueur contre Bot (JVB) et
 * désactive le mode debug.
 */
UCI::UCI()
{
    m_mainChessGame = new MainChessGame(m_gameMode);
    m_debugMode = false;

}

/**
 * @brief Gère la communication UCI avec l'interface d'échecs.
 *
 * Cette fonction lit les commandes de l'interface UCI en boucle et appelle les
 * fonctions correspondantes en fonction des commandes reçues. Elle s'arrête
 * lorsque la commande "quit" est reçue.
 */
void UCI::uciCommunication()
{
    std::string sInput;
    while(true)
    {
        std::getline(std::cin, sInput);

        if (sInput == "uci") // Receive this input at the begining
        {
            inputUCI();
        }

        else if (sInput == "isready")
        {
            std::cout << "[DEBUG] Command: isready" << std::endl;
            inputIsReady();
        }
        else if (sInput.find("debug") == 0) {
            std::istringstream ss(sInput);
            std::string cmd, debugMode;
            ss >> cmd >> debugMode;
            inputDebug(debugMode);
        }
        else if (sInput == "ucinewgame")
        {
            std::cout << "[DEBUG] Command: ucinewgame" << std::endl;
            inputUCINewGame();
        }
        else if (sInput.find("position") == 0)
        {
            std::cout << "[DEBUG] Command: position" << std::endl;
            inputPosition(sInput);
        }
        else if (sInput.find("go") == 0) // Best Move
        {
            std::cout << "[DEBUG] Command: go" << std::endl;
            inputGo(sInput);
        }
        else if (sInput == "stop")
        {
            std::cout << "[DEBUG] Command: stop" << std::endl;
            inputStop();
        }
        else if (sInput.find("setoptions") == 0)
        {
            //break; // Quitte la boucle si la commande "quit" est reçue
        }
        else if (sInput.find("register") == 0)
        {
            inputRegister(sInput);
        }
        else if (sInput == "quit")
        {
            std::cout << "[DEBUG] Command: quit" << std::endl;
            break; // Quitte la boucle si la commande "quit" est reçue
        }
    }
}


/**
 * @brief Répond à la commande UCI "uci".
 *
 * Affiche les informations sur le moteur, y compris son nom et l'auteur, puis
 * signale que le moteur est prêt avec "uciok".
 */
void UCI::inputUCI()
{
    std::cout << "id name " << m_engineName << std::endl;
    std::cout << "id author Thibaut" << std::endl;
    //Options here if needed
    std::cout << "uciok" << std::endl;
}

/**
 * @brief Répond à la commande "isready".
 *
 * Indique que le moteur est prêt en envoyant "readyok" à l'interface.
 */
void UCI::inputIsReady()
{
    std::cout << "readyok" << std::endl;
}

/**
 * @brief Active ou désactive le mode debug en fonction de l'argument fourni.
 *
 * @param debugMode "on" pour activer le mode debug, "off" pour le désactiver.
 * Si activé, des informations de débogage sont affichées.
 */
void UCI::inputDebug(const std::string &debugMode) {
    if(debugMode == "on") {
        m_debugMode = true;
        std::cout << "debug mode activated" << std::endl;
    }
    else if(debugMode == "off") {
        m_debugMode = false;
        std::cout << "debug mode deactivated" << std::endl;
    }
}

/**
 * @brief Initialise une nouvelle partie d'échecs.
 *
 * Cette fonction réinitialise l'instance de `MainChessGame` pour démarrer une
 * nouvelle partie.
 */
void UCI::inputUCINewGame()
{
    m_mainChessGame = new MainChessGame(m_gameMode);
}

/**
 * @brief Définit la position du plateau en fonction de l'entrée UCI.
 *
 * Cette fonction prend en charge la définition de la position initiale (avec
 * "startpos") ou une position spécifique en FEN (avec "fen"). Elle applique
 * ensuite une série de coups si fournie.
 *
 * @param in_sInput Commande de position, sous la forme de "position [startpos|fen] moves ..."
 */
void UCI::inputPosition(std::string &in_sInput) const {
    std::string token;
    std::istringstream ss(in_sInput);
    ss >> token; // Ignore "position"
    ss >> token;

    if (token == "startpos") {
        m_mainChessGame->getBoard()->clearBoard();
        m_mainChessGame->initChessGame();
        ss >> token; // Move on to "moves" if present
    }
    else if (token == "fen") {
        std::string fen;
        while (ss >> token && token != "moves") {
            fen += token + " ";
        }
        m_mainChessGame->setBoardFromFEN(fen);
    }

    if (token == "moves")
    {
        while(ss >> token)
        {
            std::string move = token.substr(0, 4); // Par exemple, "e2"
            char promotion = 0;

            // Si la longueur est de 5, cela signifie qu'il y a une promotion
            if (token.length() == 5) {
                promotion = token.at(4);
            }

            std::string movePlayed;
            if (!promotion) {
                m_mainChessGame->playTurn(move, promotion);
            }
            else {
                m_mainChessGame->playTurn(move);
            }
        }
    }
}

/**
 * @brief Déclenche le calcul du meilleur coup en fonction de la profondeur.
 *
 * @param in_sInput Commande "go" optionnellement suivie de "depth [n]", pour
 * spécifier la profondeur de recherche.
 */
void UCI::inputGo(std::string &in_sInput) {
    int depth = 4;
    std::istringstream iss(in_sInput);
    std::string token;

    while (iss >> token) {
        if (token == "depth") {
            iss >> depth;
        }
    }
    findBestMove(depth);
}

/**
 * @brief Arrête la recherche du meilleur coup et retourne immédiatement un coup.
 *
 * Cette fonction est généralement utilisée pour arrêter la recherche du moteur
 * en cours de calcul.
 */
//@TODO Faut coder ça
void UCI::inputStop() {
    int depth = -1;
    findBestMove(depth);
}

/**
 * @brief Recherche et envoie le meilleur coup trouvé à l'interface.
 *
 * @param depth La profondeur maximale pour la recherche du coup. Si `-1`, utilise
 * une profondeur par défaut.
 */
void UCI::findBestMove(int depth) {
    std::string bestMove = m_mainChessGame->findBestMoveForCurrentPlayer(depth);
    std::cout << "bestmove " <<bestMove<< std::endl;
}

/**
 * @brief Gère la commande "register" pour enregistrer le moteur avec un nom ou un code.
 *
 * @param in_sInput La commande "register" suivie de "name" ou "code" et les
 * informations d'enregistrement. Affiche "registration success" ou "registration error"
 * selon le résultat.
 */
void UCI::inputRegister(const std::string &in_sInput) {
    std::istringstream ss(in_sInput);
    std::string command, token;

    ss >> command;

    if (command == "later") {
        std::cout << "registration later" << std::endl;
        return;
    }

    std::string name, code;

    while (ss >> token) {
        if (token == "name") {
            ss >> name;
        }
        else if (token == "code") {
            ss >> code;
        }
    }

    if (!name.empty() || !code.empty()) {
        std::cout << "registration success" << std::endl;
        if (!name.empty()) {
            m_engineName = name;
        }
        else {
            m_engineCode = code;
        }
    }
    else {
        std::cout << "registration error" << std::endl;
    }
}


