#pragma once
#include "Logger.h"
#include <unordered_map>
#include <memory>

class LoggerManager
{
public:	
  static void SetLogFolder(std::string folder);
  static void Release();
  static Logger &GetLog(std::string name);

private:
  LoggerManager();
  ~LoggerManager();

  static std::string GetFullName( const std::string &name );

  static std::string m_logFolder;
  static std::unordered_map<std::string, std::unique_ptr<Logger>> m_logs;

};