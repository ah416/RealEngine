#include "realpch.h"

#include "Engine/Tools/Logger.h"

std::shared_ptr<Logger> Logger::s_Instance = nullptr;

void Logger::Init()
{
	s_Instance.reset(new Logger());
	s_Instance->Info("Initialized logger!");
}

Logger& Logger::Get()
{
	return *s_Instance;
}
