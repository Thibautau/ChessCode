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
    static OpeningBook* openingBook;
    static std::string repertoryPath;

    static void SetUpTestSuite() {
        // Initialisation unique de l'OpeningBook
        openingBook = new OpeningBook();
        repertoryPath = "../OpeningBook/Books/";
        bool bCouldRead = true;
        bCouldRead = bCouldRead && openingBook->getBookData(repertoryPath + "Player1.bin");
        bCouldRead = bCouldRead && openingBook->getBookData(repertoryPath + "Player2.bin");
        bCouldRead = bCouldRead && openingBook->getBookData(repertoryPath + "Player3.bin");
        bCouldRead = bCouldRead && openingBook->getBookData(repertoryPath + "Player4.bin");
        bCouldRead = bCouldRead && openingBook->getBookData(repertoryPath + "Baron30.bin");

        ASSERT_TRUE(bCouldRead) << "Failed to initialize the opening book.";
    }

    static void TearDownTestSuite() {
        // Nettoyage après l'exécution de tous les tests
        delete openingBook;
        openingBook = nullptr;
    }

    void SetUp() override {
        board.initializeBoard();
        board2.initializeBoard();
    }
};

OpeningBook* TestOpeningBook::openingBook = nullptr;
std::string TestOpeningBook::repertoryPath = "../OpeningBook/Books/";

TEST_F(TestOpeningBook, readOpeningBook)
{
    OpeningBook* openingBook = new OpeningBook();
    bool bCouldRead = true;
    bCouldRead = bCouldRead && openingBook->getBookData(repertoryPath + "Player1.bin");
    bCouldRead = bCouldRead && openingBook->getBookData(repertoryPath + "Player2.bin");
    bCouldRead = bCouldRead && openingBook->getBookData(repertoryPath + "Player3.bin");
    bCouldRead = bCouldRead && openingBook->getBookData(repertoryPath + "Player4.bin");
    bCouldRead = bCouldRead && openingBook->getBookData(repertoryPath + "Baron30.bin");

    EXPECT_TRUE(bCouldRead);
}

TEST_F(TestOpeningBook, getBestMoveForHash)
{
    OpeningBook* openingBook = new OpeningBook();
    bool bCouldRead = openingBook->getBookData(repertoryPath + "Player1.bin");

    std::string sBestMove;
    std::string sBestMove2;
    openingBook->getBestMoveForHash(sBestMove, 295561130868254);
    openingBook->getBestMoveForHash(sBestMove2, 2412650173551080);

    EXPECT_TRUE(bCouldRead);
    EXPECT_EQ(sBestMove, "g5f6");
    EXPECT_EQ(sBestMove2, "c2d3");

}