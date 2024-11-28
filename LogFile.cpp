#include <LogFile.h>




LogFile::LogFile(std::string in_sfileName, bool in_bWriteLog)
{
	m_sFileName = in_sfileName;
	m_sRepertoryPath = "../Log/" + in_sfileName;
	m_bWriteLog = in_bWriteLog;

	if (!std::filesystem::exists("../Log")) {
		std::filesystem::create_directory("../Log");
	}

	// Initialiser la file d'attente et le nombre de threads pour l'écriture asynchrone
	spdlog::init_thread_pool(16384, 1);  // 16384 éléments dans la file d'attente, 1 thread d'écriture

	// Créer un logger asynchrone qui écrit dans un fichier
	spdlog::drop("async_logger");
	m_logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_logger", m_sRepertoryPath);
	spdlog::set_default_logger(m_logger);
}

void LogFile::clear()
{
	// Ouvre le fichier en mode "truncation" (efface le contenu)
	std::ofstream ofs(m_sRepertoryPath, std::ofstream::trunc);
	ofs.close();  // Ferme immédiatement le fichier

	// Crée un nouveau logger avec le fichier réinitialisé (contenu effacé)
	spdlog::drop("async_logger");
	m_logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_logger", m_sRepertoryPath);
	spdlog::set_default_logger(m_logger);
}

void LogFile::close()
{
	spdlog::shutdown();
}

void LogFile::log(const std::string& message) const
{
	if(m_bWriteLog) // Si on veut utiliser le système de log, sinon on ne fait rien
	{
		m_logger->info(message);
	}
}

void LogFile::logError(const std::string& message) const
{
	if(m_bWriteLog) // Si on veut utiliser le système de log, sinon on ne fait rien
	{
		m_logger->error("[ERROR] " + message);
	}
}

void LogFile::logInfo(const std::string& message) const
{
	if(m_bWriteLog) // Si on veut utiliser le système de log, sinon on ne fait rien
	{
		m_logger->info("[INFO] " + message);
	}
}

bool LogFile::isWrittigAllowed() const
{
	return m_bWriteLog;
}

void LogFile::setWrittigAllowed(bool in_bIsWrittingAllowed)
{
	m_bWriteLog = in_bIsWrittingAllowed;
}

