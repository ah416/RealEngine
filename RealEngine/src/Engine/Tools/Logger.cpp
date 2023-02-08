#include "Logger.h"

Logger* Logger::s_Instance = nullptr;

void Logger::Init()
{
	s_Instance = new Logger();
	s_Instance->Info("Initialized logger!");
}

Logger& Logger::Get()
{
	return *s_Instance;
}
