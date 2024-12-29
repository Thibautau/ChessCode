//
// Created by thiba on 29/12/2024.
//

#ifndef OPENINGBOOK_H
#define OPENINGBOOK_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>


struct ZobristKeyData;

class OpeningBook {

private:
    //For each zobrist key, associates an array of ZobristKeyData (moves + nbOfUses by the creator of the book)
    std::map<uint64_t, std::vector<ZobristKeyData>> m_openingBook;

public:
    OpeningBook();
    ~OpeningBook();

    void clear();

    bool getBookData(const std::string &in_sFilePath);

    bool getBestMoveForHash(std::string &out_sBestMove, uint64_t in_uiZobristKey) const;
};



#endif //OPENINGBOOK_H
