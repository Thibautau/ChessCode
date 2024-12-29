//
// Created by thiba on 29/12/2024.
//

#ifndef OPENINGBOOK_H
#define OPENINGBOOK_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>


struct Entry;

class OpeningBook {

private:
    std::map<uint64_t, Entry> m_book;

public:
    OpeningBook();
    ~OpeningBook();

    void getBookData(const std::string &in_sFilePath);

};



#endif //OPENINGBOOK_H
