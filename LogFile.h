//
// Created by Laurent on 26/11/2024.
//

#ifndef LOGINFILE_H
#define LOGINFILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>  // Pour vérifier l'existence du dossie
#include <spdlog/spdlog.h>            // Inclusion du header principal de spdlog
#include <spdlog/sinks/basic_file_sink.h>  // Inclusion du sink pour les fichiers (si vous voulez écrire dans un fichier)
#include <spdlog/async.h>


class LogFile {
private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::string m_sFileName;
    std::string m_sRepertoryPath; // Directory Path where the logFile is stocked
    bool m_bWriteLog = false; // To know if we open/clear/write logs or not

public:
    LogFile(std::string in_sfileName, bool in_bWriteLog);

    void clear();
    void close();

    void log(const std::string& message) const;
    void logError(const std::string& message) const;
    void logInfo(const std::string& message) const;

    bool isWrittigAllowed() const;
    void setWrittigAllowed(bool in_bIsWrittingAllowed);
};



#endif //LOGINFILE_H
