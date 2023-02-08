#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Format.h"

#include <iostream>
#include <source_location>
#include <utility>

class Logger
{
	enum class LogLevel
	{
		Trace = 0, Info, Warn, Error, Critical
	};

public:
	~Logger() { delete s_Instance; }

	static void Init();

	static Logger& Get();

	template<typename... Args>
	constexpr void Info(Args&&... args);

	template<typename FormatString, typename... Args>
	constexpr void Info(const FormatString& str, Args&&... args);

	template<typename... Args>
	constexpr void Warn(Args&&... args);

	template<typename FormatString, typename... Args>
	constexpr void Warn(const FormatString& str, Args&&... args);

	template<typename... Args>
	constexpr void Error(Args&&... args);

	template<typename FormatString, typename... Args>
	constexpr void Error(const FormatString& str, Args&&... args);

	template<typename... Args>
	constexpr void Critical(Args&&... args);

	template<typename FormatString, typename... Args>
	constexpr void Critical(const FormatString& str, Args&&... args);
private:
	Logger() = default;

	template<typename... Args>
	constexpr void Log(LogLevel lvl, Args&&...);

	template<typename FormatString, typename... Args>
	constexpr void Log(LogLevel lvl, const FormatString& str, Args&&... args);
private:
	static Logger* s_Instance;
};

template<typename... Args>
constexpr void Logger::Info(Args&&... args)
{
	Log(LogLevel::Info, args...);
}

template<typename FormatString, typename... Args>
constexpr void Logger::Info(const FormatString& str, Args&&... args)
{
	Log(LogLevel::Info, str, args...);
}

template<typename... Args>
constexpr void Logger::Warn(Args&&... args)
{
	Log(LogLevel::Warn, args...);
}

template<typename FormatString, typename... Args>
constexpr void Logger::Warn(const FormatString& str, Args&&... args)
{
	Log(LogLevel::Warn, str, args...);
}

template<typename... Args>
constexpr void Logger::Error(Args&&... args)
{
	Log(LogLevel::Error, args...);
}

template<typename FormatString, typename... Args>
constexpr void Logger::Error(const FormatString& str, Args&&... args)
{
	Log(LogLevel::Error, str, args...);
}

template<typename... Args>
constexpr void Logger::Critical(Args&&... args)
{
	Log(LogLevel::Critical, args...);
}

template<typename FormatString, typename... Args>
constexpr void Logger::Critical(const FormatString& str, Args&&... args)
{
	Log(LogLevel::Critical, str, args...);
}

template<typename...Args>
inline constexpr void Logger::Log(LogLevel lvl, Args&&... args)
{
	std::string fmt_str = "{}";
	for (int i = 1; i < sizeof...(Args); i++)
	{
		fmt_str += " {}";
	}
	fmt_str += '\n';
	std::string formatted_string = fmt::format(fmt_str, std::forward<Args>(args)...);
#ifdef _WIN32
	auto stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(stdHandle, formatted_string.c_str(), (DWORD)formatted_string.size(), nullptr, nullptr);
#else
	std::cout << formatted_string;
#endif
}

template<typename FormatString, typename... Args>
inline constexpr void Logger::Log(LogLevel lvl, const FormatString& str, Args&&... args)
{
	std::string formatted_string = str;
	if constexpr (sizeof...(args) > 0)
		formatted_string = fmt::format(str, std::forward<Args>(args)...);
	formatted_string += '\n';

#ifdef _WIN32
	auto stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(stdHandle, formatted_string.c_str(), (DWORD)formatted_string.size(), nullptr, nullptr);
#else
	std::cout << formatted_string;
#endif
}
