//
// Created by thiba on 29/12/2024.
//

#include "ReadOpeningBook.h"

#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>



// Fonction pour convertir un coup binaire en un format lisible
std::string ReadOpeningBook::decode_move(uint16_t move) {
    int from_square = (move >> 6) & 0x3F;  // Les 6 bits de poids faible
    int to_square = move & 0x3F;          // Les 6 bits suivants
    int promotion = (move >> 12) & 0x7;   // Les 3 bits de promotion

    auto square_name = [](int square) -> std::string {
        std::string files = "abcdefgh";
        std::string ranks = "12345678";
        return std::string(1, files[square % 8]) + ranks[square / 8];
    };

    std::string move_str = square_name(from_square) + square_name(to_square);
    if (promotion) {
        std::string promotions = "qrbn";
        move_str += promotions[promotion - 1];
    }
    return move_str;
}

// Fonction pour lire et analyser un fichier binaire Polyglot
std::vector<Entry> ReadOpeningBook::read_polyglot(const std::string& file_path) {
    std::vector<Entry> entries;
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + file_path);
    }

    while (file) {
        uint64_t zobrist_key;
        uint16_t move, weight, score, learn;

        // Lire une entrée de 16 octets
        file.read(reinterpret_cast<char*>(&zobrist_key), sizeof(zobrist_key));
        file.read(reinterpret_cast<char*>(&move), sizeof(move));
        file.read(reinterpret_cast<char*>(&weight), sizeof(weight));
        file.read(reinterpret_cast<char*>(&score), sizeof(score));
        file.read(reinterpret_cast<char*>(&learn), sizeof(learn));

        if (file.gcount() < 16) {
            break;  // Ignore les entrées partielles
        }

        // Ajouter l'entrée décodée
        entries.push_back({
            zobrist_key,
            decode_move(move),
            weight,
            score,
            learn
        });
    }

    file.close();
    return entries;
}

// Affichage des entrées
void ReadOpeningBook::print_entries(const std::vector<Entry>& entries, size_t limit) {
    std::cout << "Affichage des premières entrées du livre d'ouverture :\n";
    for (size_t i = 0; i < entries.size() && i < limit; ++i) {
        const auto& entry = entries[i];
        std::cout << "Zobrist Key: " << std::hex << entry.zobrist_key << std::dec
                  << ", Move: " << entry.move
                  << ", Weight: " << entry.weight
                  << ", Score: " << entry.score
                  << ", Learn: " << entry.learn << '\n';
    }
}
