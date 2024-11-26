//
// Created by Laurent on 26/11/2024.
//

#include "LogInFile.h"

LogInFile::LogInFile(std::string in_sfileName)
{
    m_sFileName = "../Log/" + in_sfileName;

    // Vérifier si le dossier 'Log/' existe sinon le créer
    if (!std::filesystem::exists("Log")) {
        std::filesystem::create_directory("Log");
    }

    // Ouvrir un fichier de log en mode écriture (création du fichier si inexistant)
    // Ouvrir un fichier de log en mode écriture (création du fichier si inexistant)
    m_logFile.open(m_sFileName, std::ios::out | std::ios::app); // 'std::ios::app' pour ajouter sans écraser
    if (!m_logFile.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier de log : " << m_sFileName << std::endl;
    }
    //std::ofstream m_logFile(m_sFileName, std::ios::out | std::ios::app);  // 'std::ios::app' pour ajouter sans écraser
}

void LogInFile::writeLogToNextLine(const std::string& in_sLogMessage)
{
    if (!m_logFile) {
        std::cerr << "Erreur lors de l'ouverture du fichier de log !" << std::endl;
        return;
    }

    // Écrire des informations dans le fichier de log
    m_logFile << in_sLogMessage << "\n";
}

void LogInFile::close()
{
    // Fermer le fichier après utilisation
    m_logFile.close();
}

std::string LogInFile::getFileName() const
{
    return m_sFileName;
}
