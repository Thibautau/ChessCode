//
// Created by thiba on 29/12/2024.
//

#include "OpeningBook.h"

#include <iostream>
#include <limits>

#include "ReadOpeningBook.h"

OpeningBook::OpeningBook()
{

}

OpeningBook::~OpeningBook()
{

}

void OpeningBook::clear()
{
    m_openingBook.clear();
}

bool OpeningBook::getBookData(const std::string& in_sFilePath)
{
    try {
        ReadOpeningBook::read_polyglot(in_sFilePath, m_openingBook);
        return true;
    } catch (const std::exception& ex) {
        std::cerr << "Erreur dans getBookData : " << ex.what() << std::endl;
        return false;
    }
}

bool OpeningBook::getBestMoveForHash(std::string& out_sBestMove, uint64_t in_uiZobristKey) const {
    if(! m_openingBook.contains(in_uiZobristKey))
    {
        out_sBestMove = "";
        return false;
    }

    std::vector<ZobristKeyData> zobristKeyData = m_openingBook.at(in_uiZobristKey);
    if(zobristKeyData.empty())
    {
        out_sBestMove = "";
        return false;
    }

    uint16_t nbUsesMax = std::numeric_limits<uint16_t>::min();
    std::string bestMove;
    for(const ZobristKeyData& data : zobristKeyData)
    {
        if(data.weight > nbUsesMax)
        {
            nbUsesMax = data.weight;
            bestMove = data.move;
        }
    }

    out_sBestMove = bestMove;
    return ! bestMove.empty();
}


