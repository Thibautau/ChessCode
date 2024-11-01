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


public:
    UCI();

    static std::string m_engineName;
    static std::string m_engineCode;
    static GameMode m_gameMode;
    void uciCommunication();
    void inputUCI();
    void inputIsReady();
    void inputUCINewGame();
    void inputPosition(std::string &in_sInput) const;
    void inputGo(std::string &in_sInput);
    void inputStop();
    void findBestMove(int depth);
    void inputRegister(const std::string &in_sInput);
};



#endif //UCI_H
