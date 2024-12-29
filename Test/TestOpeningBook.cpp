//
// Created by thiba on 29/12/2024.
//

#include "TestOpeningBook.h"
#include <gtest/gtest.h>
#include "Board.h"
#include "OpeningBook/OpeningBook.h"


class TestOpeningBook : public ::testing::Test {
protected:
    Board board;
    Board board2;
    std::string repertoryPath = "../OpeningBook/Books/";

    void SetUp() override {
        board.initializeBoard();
        board2.initializeBoard();
    }
};

TEST_F(TestOpeningBook, readOpeningBook)
{
    OpeningBook* openingBook = new OpeningBook();
    openingBook->getBookData( repertoryPath + "Player1.bin");
    openingBook->getBookData(repertoryPath + "Player2.bin");
    openingBook->getBookData(repertoryPath + "Player3.bin");
    openingBook->getBookData(repertoryPath + "Player4.bin");
    openingBook->getBookData(repertoryPath + "Baron30.bin");

}