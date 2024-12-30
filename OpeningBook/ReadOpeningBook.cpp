//
// Created by thiba on 29/12/2024.
//

#include "ReadOpeningBook.h"

#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <stdexcept>


// Fonction pour convertir un coup binaire en un format lisible
std::string ReadOpeningBook::decode_move(uint16_t move) {
    // Extraire les composants du mouvement à partir du format Polyglot
    int from_square = (move >> 6) & 0x3F;  // Bits 6-11 pour la case source
    int to_square = move & 0x3F;          // Bits 0-5 pour la case destination
    int promotion = (move >> 12) & 0x7;   // Bits 12-14 pour la promotion

    // Convertir un numéro de case en une chaîne de type "e2"
    auto square_name = [](int square) -> std::string {
        const std::string files = "abcdefgh";
        const std::string ranks = "12345678";
        return std::string(1, files[square % 8]) + ranks[square / 8];
    };

    // Construire la chaîne de mouvement
    std::string move_str = square_name(from_square) + square_name(to_square);

    // Ajouter une promotion si elle existe
    if (promotion > 0 && promotion <= 4) {
        const std::string promotions = "qrbn";  // Reine, tour, fou, cavalier
        move_str += promotions[promotion - 1];
    }

    return move_str;
}


// Fonction pour lire et analyser un fichier binaire Polyglot
void ReadOpeningBook::read_polyglot(const std::string& in_sFilePath, std::map<uint64_t, std::vector<ZobristKeyData>>& out_bookData)
{
    std::ifstream file(in_sFilePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + in_sFilePath);
    }

    while (file) {
        uint64_t zobrist_key;
        uint16_t move, weight, score, learn;

        // Lire un bloc de 16 octets
        file.read(reinterpret_cast<char*>(&zobrist_key), sizeof(zobrist_key));
        if (file.gcount() != sizeof(zobrist_key)) {
            break; // Arrêter si un bloc incomplet est lu
        }

        file.read(reinterpret_cast<char*>(&move), sizeof(move));
        if (file.gcount() != sizeof(move)) {
            break;
        }

        file.read(reinterpret_cast<char*>(&weight), sizeof(weight));
        if (file.gcount() != sizeof(weight)) {
            break;
        }

        file.read(reinterpret_cast<char*>(&score), sizeof(score));
        if (file.gcount() != sizeof(score)) {
            break;
        }

        file.read(reinterpret_cast<char*>(&learn), sizeof(learn));
        if (file.gcount() != sizeof(learn)) {
            break;
        }

        zobrist_key = convertuint64ToHostEndian(zobrist_key);
        move = convertuint16ToHostEndian(move); // Convertir en big-endian
        weight = convertuint16ToHostEndian(weight); // Convertir en big-endian
        score = convertuint16ToHostEndian(score); // Convertir en big-endian
        learn = convertuint16ToHostEndian(learn); // Convertir en big-endian
        ZobristKeyData data = {decode_move(move), weight, score, learn};

        // Ajouter l'entrée décodée
        // Si la clé existe déjà, on ne fait que rajouter ZobristKeyData
        if(out_bookData.contains(zobrist_key))
        {
            std::vector<ZobristKeyData> bookData = out_bookData.at(zobrist_key);
            bookData.push_back(data);
        }
        else // Si la clé n'existe pas, on l'initialise
        {
            out_bookData[zobrist_key].push_back(data);
        }
    }

    file.close();
}



// Affichage des entrées
void ReadOpeningBook::print_entries(const std::map<uint64_t, std::vector<ZobristKeyData>>& out_bookData, size_t limit) {
    std::cout << "Affichage des premières entrées du livre d'ouverture :\n";

    size_t count = 0; // Compteur pour limiter le nombre total d'entrées affichées
    for (const auto& [zobrist_key, entries] : out_bookData) { // Itération sur la map
        if (entries.empty()) {
            continue; // Ignorer les clés avec des vecteurs vides
        }

        for (const auto& entry : entries) { // Itération sur le vecteur associé à la clé
            std::cout << "Zobrist Key: " << std::hex << zobrist_key << std::dec
                      << ", Move: " << entry.move
                      << ", Weight: " << entry.weight
                      << ", Score: " << entry.score
                      << ", Learn: " << entry.learn << '\n';
            count++;

            // Arrêter si la limite est atteinte
            if (count >= limit) {
                return;
            }
        }
    }
}

uint64_t ReadOpeningBook::convertuint64ToHostEndian(uint64_t val) {
    return ((val & 0xFF00000000000000ULL) >> 56) |
           ((val & 0x00FF000000000000ULL) >> 40) |
           ((val & 0x0000FF0000000000ULL) >> 24) |
           ((val & 0x000000FF00000000ULL) >> 8)  |
           ((val & 0x00000000FF000000ULL) << 8)  |
           ((val & 0x0000000000FF0000ULL) << 24) |
           ((val & 0x000000000000FF00ULL) << 40) |
           ((val & 0x00000000000000FFULL) << 56);
}


uint16_t ReadOpeningBook::convertuint16ToHostEndian(uint16_t netshort) {
    return (netshort >> 8) | (netshort << 8);
}