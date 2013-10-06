#include "Utility/LoggerManager.h"

std::string LoggerManager::m_logFolder = "";
std::unordered_map<std::string, std::unique_ptr<Logger>> LoggerManager::m_logs;

std::string LoggerManager::GetFullName( const std::string &name )
{
  return m_logFolder + "/" + name;	return name;
}

Logger & LoggerManager::GetLog( std::string name )
{
  name = GetFullName(name);

  if (m_logs.count(name) == 0)
  {
    m_logs[name].reset(new Logger());
    m_logs[name]->Open(name);
    m_logs[name]->AddTimeStamps(true);
    m_logs[name]->SetLoggingLevel(LogLevel::ALL);
  }

  return *m_logs[name];
}

void LoggerManager::Release()
{
  m_logs.clear();
}

LoggerManager::LoggerManager()
{

}

void LoggerManager::SetLogFolder( std::string folder )
{
  m_logFolder = folder;
}

LoggerManager::~LoggerManager()
{
  m_logs.clear();
}
