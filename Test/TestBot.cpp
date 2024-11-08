//
// Created by thiba on 07/11/2024.
//

#include <iostream>
#include <chrono>
#include "Bot.h"
#include "Board.h"
#include "Zobrist.h"
#include <gtest/gtest.h>
#include "MainChessGame.h"


class TestBot : public ::testing::Test {
protected:
    MainChessGame* game = nullptr;
    Board* board = nullptr;
    Bot* whiteBot = nullptr;
    Bot* blackBot = nullptr;

    void SetUp() override {
        game = new MainChessGame(GameMode::BVJ);
        game->initChessGame();
        board = game->getBoard();
        whiteBot = new Bot(Color::WHITE);
        blackBot = new Bot(Color::BLACK);
    }
};