//
// Created by thiba on 29/12/2024.
//

#ifndef READOPENINGBOOK_H
#define READOPENINGBOOK_H
#include <cstdint>
#include <string>
#include <vector>


struct Entry {
    uint64_t zobrist_key;
    std::string move;
    uint16_t weight;
    uint16_t score;
    uint16_t learn;
};


class ReadOpeningBook {

public:
    static std::string decode_move(uint16_t move);
    static std::vector<Entry> read_polyglot(const std::string& file_path);
    static void print_entries(const std::vector<Entry>& entries, size_t limit = 10);

};


#endif //READOPENINGBOOK_H
