//
// Created by thiba on 29/10/2024.
//

#ifndef UCI_H
#define UCI_H
#include <string>


class UCI {

public:
    static std::string ENGINENAME;

    static void uciCommunication();
    static void inputUCI();
    static void inputIsReady();
    static void inputUCINewGame();
    static void inputPosition();
    static void inputGo();
};



#endif //UCI_H
