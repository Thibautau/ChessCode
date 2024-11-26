//
// Created by Laurent on 26/11/2024.
//

#ifndef LOGINFILE_H
#define LOGINFILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>  // Pour vérifier l'existence du dossie




class LogInFile {
private:
    std::string m_sFileName;
    std::ofstream m_logFile;

public:
    LogInFile(std::string in_sfileName);
    void writeLogToNextLine(const std::string& in_sLogMessage);
    void close();
    std::string getFileName() const;
};



#endif //LOGINFILE_H
