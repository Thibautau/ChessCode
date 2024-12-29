//
// Created by thiba on 29/12/2024.
//

#ifndef READOPENINGBOOK_H
#define READOPENINGBOOK_H
#include <cstdint>
#include <map>
#include <string>
#include <vector>


struct Entry
{
    uint64_t zobrist_key;
    std::string move;
    uint16_t weight;
    uint16_t score;
    uint16_t learn;
};

struct ZobristKeyData
{
    std::string move;
    uint16_t weight;
    uint16_t score;
    uint16_t learn;
};


class ReadOpeningBook {

public:
    static std::string decode_move(uint16_t move);
    static void read_polyglot(const std::string &in_sFilePath, std::map<uint64_t, std::vector<ZobristKeyData>> &out_bookData);
    static void print_entries(const std::map<uint64_t, std::vector<ZobristKeyData>> &out_bookData, size_t limit = 10);

    static uint64_t convertuint64ToHostEndian(uint64_t val);

    static uint16_t convertuint16ToHostEndian(uint16_t netshort);
};


#endif //READOPENINGBOOK_H
