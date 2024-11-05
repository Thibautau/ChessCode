//
// Created by thiba on 29/10/2024.
//

#include "UCI.h"

#include <iostream>
#include <sstream>

std::string UCI::m_engineName = "MonEngine v1";
std::string UCI::m_engineCode = "";
GameMode UCI::m_gameMode = GameMode::JVB;
UCI::UCI()
{
    m_mainChessGame = new MainChessGame(m_gameMode);
    m_debugMode = false;

}

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

void UCI::inputUCI()
{
    std::cout << "id name " << m_engineName << std::endl;
    std::cout << "id author Thibaut" << std::endl;
    //Options here if needed
    std::cout << "uciok" << std::endl;
}

void UCI::inputIsReady()
{
    std::cout << "readyok" << std::endl;
}

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

void UCI::inputUCINewGame()
{
    m_mainChessGame = new MainChessGame(m_gameMode);
}

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


void UCI::inputStop() {
    int depth = -1;
    findBestMove(depth);
}

void UCI::findBestMove(int depth) {
    std::string bestMove = m_mainChessGame->findBestMoveForCurrentPlayer(depth);
    std::cout << "bestmove " <<bestMove<< std::endl;
}


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


