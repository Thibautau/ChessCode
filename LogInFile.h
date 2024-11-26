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
    std::string m_sRepertoryPath; // Directory Path where the logFile is stocked
    std::ofstream m_logFile;
    bool m_bWriteLog = false; // To know if we open/clear/write logs or not

public:
    LogInFile(std::string in_sfileName, bool in_bWriteLog);

    /**
    * Write the message into the logFile. It writes on the next Line.
    * /!\ You should put a "\n" at the end of your message or use another time this fonction
    */
    bool writeLogOnNextLine(const std::string& in_sLogMessage);
    /**
    * Write the message into the logFile. Ca écrit à la suite.
    * /!\ You should put a "\n" at the end of your message to write the message in the logFile.
    */
    bool writeLogOnSameLine(const std::string& in_sLogMessage);
    /**
    * Close the log file if it is open
    */
    void close();
    /**
    * Clear the log file if it is open
    */
    bool clear();
    /**
    *  Open the logFile it not already with write access
    */
    bool openFileWithWriteAccess();

    std::string getFileName() const;
    std::string getRepertoryPath() const;
    bool isWrittigAllowed() const;
    void setWrittigAllowed(bool in_bIsWrittingAllowed);
};



#endif //LOGINFILE_H
