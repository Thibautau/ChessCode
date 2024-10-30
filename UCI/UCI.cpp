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
            std::cout << "id name " << ENGINENAME << std::endl;
            std::cout << "id author Thibaut" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        else if (sInput == "quit")
        {
            break; // Quitte la boucle si la commande "quit" est reçue
        }
        else if (sInput == "isready")
        {
            std::cout << "readyok" << std::endl;
        }
        else if (sInput == "position")
        {
            //break; // Quitte la boucle si la commande "quit" est reçue
        }
        else if (sInput == "go")
        {
            //break; // Quitte la boucle si la commande "quit" est reçue
        }
        else if (sInput == "stop")
        {
            //break; // Quitte la boucle si la commande "quit" est reçue
        }
        else if (sInput == "bestmove")
        {
            //break; // Quitte la boucle si la commande "quit" est reçue
        }
        else if (sInput == "ucinewgame")
        {
            //break; // Quitte la boucle si la commande "quit" est reçue
        }
        else if (sInput == "setoptions")
        {
            //break; // Quitte la boucle si la commande "quit" est reçue
        }
    }
}
