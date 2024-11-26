//
// Created by Laurent on 26/11/2024.
//

#include "LogInFile.h"

LogInFile::LogInFile(std::string in_sfileName, bool in_bWriteLog)
{
  	m_bWriteLog = in_bWriteLog;
    m_sRepertoryPath = "../Log/" + in_sfileName;
    m_sFileName = in_sfileName;

    // Vérifier si le dossier 'Log/' existe sinon le créer
    if (!std::filesystem::exists("../Log")) {
        std::filesystem::create_directory("../Log");
    }
}

bool LogInFile::openFileWithWriteAccess()
{
  	if(! m_bWriteLog) // Si on ne veut pas utiliser le système de log, on ne fait rien
  	{
        return false;
  	}
  	if (m_logFile.is_open()) // Si c'est déjà ouvert, on ne fait rien
  	{
        return true;
    }

  	// Ouvrir un fichier de log en mode écriture (création du fichier si inexistant)
	m_logFile.open(m_sRepertoryPath, std::ios::out | std::ios::app); // 'std::ios::app' pour ajouter sans écraser
    if (!m_logFile.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier de log : " << m_sFileName << std::endl;
        return false;
    }

    return true;
}

bool LogInFile::writeLogOnNextLine(const std::string& in_sLogMessage)
{
    return writeLogOnSameLine("\n" + in_sLogMessage);
}

bool LogInFile::writeLogOnSameLine(const std::string& in_sLogMessage)
{
  	if(! m_bWriteLog) // Si on ne veut pas utiliser le système de log, on ne fait rien
  	{
        return false;
  	}

  	// If it is already open, it just return true
    bool bFileOpen = openFileWithWriteAccess();

    if(! bFileOpen) // Si ça n'a toujours pas réussi à l'ouvrir
    {
      return false;
    }

    // Écrire des informations dans le fichier de log
    m_logFile << in_sLogMessage;
    return true;
}

void LogInFile::close()
{
  	if(! m_bWriteLog) // Si on ne veut pas utiliser le système de log, on ne fait rien
  	{
        return;
  	}

    if (m_logFile.is_open()) // Si c'est déjà ouvert, on ne fait rien
  	{
        m_logFile.close();
    }
    else
    {
      	std::cerr << "Le fichier est déjà fermé!" << std::endl;
    }
}

bool LogInFile::clear()
{
    bool bIsFileOpen = openFileWithWriteAccess();
    if(!bIsFileOpen)
    {
      return false;
    }

    m_logFile.clear();
    return true;
}

std::string LogInFile::getFileName() const
{
    return m_sFileName;
}

std::string LogInFile::getRepertoryPath() const
{
    return m_sRepertoryPath;
}

bool LogInFile::isWrittigAllowed() const
{
  	return m_bWriteLog;
}

void LogInFile::setWrittigAllowed(bool in_bIsWrittingAllowed)
{
	m_bWriteLog = in_bIsWrittingAllowed;
}
