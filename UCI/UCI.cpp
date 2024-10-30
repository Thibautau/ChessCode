//
// Created by thiba on 29/10/2024.
//

#include "UCI.h"

#include <iostream>


std::string UCI::ENGINENAME = "MonEngine1";

void UCI::uciCommunication()
{


    std::string sInput;
    while(true)
    {
        std::getline(std::cin, sInput);

        if (sInput == "uci") // Receive this input at the begining
        {
            std::cout << "id name " << ENGINENAME << std::endl;
            std::cout << "id author Thiba" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        else if (sInput == "quit")
        {
            break; // Quitte la boucle si la commande "quit" est reçue
        }
    }
}
