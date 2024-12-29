//
// Created by thiba on 29/12/2024.
//

#include "OpeningBook.h"

#include <iostream>

#include "ReadOpeningBook.h"

OpeningBook::OpeningBook()
{

}

OpeningBook::~OpeningBook()
{

}

void OpeningBook::getBookData(const std::string& in_sFilePath)
{
    try {
        std::vector<Entry> entries = ReadOpeningBook::read_polyglot(in_sFilePath);
        ReadOpeningBook::print_entries(entries, 10000);
    } catch (const std::exception& ex) {
        std::cerr << "Erreur dans getBookData : " << ex.what() << std::endl;
    }
}



