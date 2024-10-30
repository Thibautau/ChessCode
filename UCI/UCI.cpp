//
// Created by thiba on 29/10/2024.
//

#include "UCI.h"

#include <iostream>


std::string UCI::ENGINENAME = "MonEngine v1";

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
            inputIsReady();
        }
        else if (sInput == "ucinewgame")
        {
            inputUCINewGame();
        }
        else if (sInput == "position")
        {
            inputPosition();
        }
        else if (sInput == "go") // Best Move
        {
            inputGo();
        }
        else if (sInput == "stop")
        {
            //break; // Quitte la boucle si la commande "quit" est reçue
        }
        else if (sInput == "bestmove")
        {
            //break; // Quitte la boucle si la commande "quit" est reçue
        }
        else if (sInput == "setoptions")
        {
            //break; // Quitte la boucle si la commande "quit" est reçue
        }
        else if (sInput == "quit")
        {
            break; // Quitte la boucle si la commande "quit" est reçue
        }
    }
}

void UCI::inputUCI()
{
    std::cout << "id name " << ENGINENAME << std::endl;
    std::cout << "id author Thibaut" << std::endl;
    //Options here if needed
    std::cout << "uciok" << std::endl;
}

void UCI::inputIsReady()
{
    std::cout << "readyok" << std::endl;
}

void UCI::inputUCINewGame()
{

}

void UCI::inputPosition()
{

}

void UCI::inputGo()
{

}
