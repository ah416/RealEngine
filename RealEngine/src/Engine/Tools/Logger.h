#pragma once

#include "Engine/Instrumentor.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <memory>
#include <iostream>
#include <string>
#include <format>
#include <source_location>
#include <filesystem>

class Logger
{
	enum class LogLevel
	{
		Trace = 0, Info, Warn, Error, Critical
	};

public:
	static void Init();

	static Logger& Get();

	template<typename... Args>
	inline constexpr void Info(Args&&... items);

	template<typename... Args>
	inline constexpr void Warn(Args&&... item);

	template<typename... Args>
	inline constexpr void Error(Args&&... item);

	template<typename... Args>
	inline constexpr void Critical(Args&&... item);

	template<typename FormatString, typename... Args>
	inline constexpr void Info(const FormatString& str, Args&&... items);

	template<typename FormatString, typename... Args>
	inline constexpr void Warn(const FormatString& str, Args&&... item);

	template<typename FormatString, typename... Args>
	inline constexpr void Error(const FormatString& str, Args&&... item);

	template<typename FormatString, typename... Args>
	inline constexpr void Critical(const FormatString& str, Args&&... item);

private:
	Logger() = default;

	// Returns the text color for the given LogLevel
	constexpr std::pair<std::string, std::string> GetLvlColor(LogLevel lvl);

	template<typename... Args>
	constexpr void Log(LogLevel lvl, Args&&... args);

	template<typename FormatString, typename... Args>
	constexpr void Log(LogLevel lvl, const FormatString& str, Args&&... args);

private:
	static std::shared_ptr<Logger> s_Instance;
};

constexpr std::pair<std::string, std::string> Logger::GetLvlColor(LogLevel lvl)
{

	switch (lvl)
	{
	case LogLevel::Info:
		return { "\x1B[32m", "\033[0m" };
	case LogLevel::Warn:
		return { "\x1B[93m", "\033[0m" };
	case LogLevel::Error:
		return { "\x1B[31m", "\033[0m" };
	case LogLevel::Critical:
		return { "\u001b[41;1m", "\033[0m" };
	default:
		return { "\x1B[32m", "\033[0m" };
	}
}

template<typename... Args>
inline constexpr void Logger::Log(LogLevel lvl, Args&&... args)
{
	if (sizeof...(args) <= 0) return;

	auto [start_color, end_color] = GetLvlColor(lvl);

	std::string fmt_str = "{}";
	for (int i = 1; i < sizeof...(Args); i++)
	{
		fmt_str += " {}";
	}
	fmt_str = start_color + fmt_str + end_color + '\n';
	std::string str = std::vformat(fmt_str, std::make_format_args(args...));

#ifdef _WIN32
	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(stdOut, str.c_str(), (DWORD)str.size(), nullptr, nullptr);
#else
	std::cout << str << '\n';
#endif
}

template<typename FormatString, typename... Args>
constexpr void Logger::Log(LogLevel lvl, const FormatString& str, Args&&... args)
{
	std::string formatted_string;
	auto [start_color, end_color] = GetLvlColor(lvl);
	if (sizeof...(args) > 0)
	{
		formatted_string = std::vformat(str, std::make_format_args(std::forward<Args>(args)...));
	}
	else
		formatted_string = str;

	formatted_string = start_color + formatted_string + end_color + '\n';

#ifdef _WIN32
	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(stdOut, formatted_string.c_str(), (DWORD)formatted_string.size(), nullptr, nullptr);
#else
	std::cout << formatted_string << '\n';
#endif
}

template<typename... Args>
inline constexpr void Logger::Info(Args&&... item)
{
	Log(LogLevel::Info, std::forward<Args>(item)...);
}

template<typename... Args>
inline constexpr void Logger::Warn(Args&&... item)
{
	Log(LogLevel::Warn, std::forward<Args>(item)...);
}

template<typename... Args>
inline constexpr void Logger::Error(Args&&... item)
{
	Log(LogLevel::Error, std::forward<Args>(item)...);
}

template<typename... Args>
inline constexpr void Logger::Critical(Args&&... item)
{
	Log(LogLevel::Critical, std::forward<Args>(item)...);
}

template<typename FormatString, typename... Args>
inline constexpr void Logger::Info(const FormatString& str, Args&&... item)
{
	Log(LogLevel::Info, str, std::forward<Args>(item)...);
}

template<typename FormatString, typename... Args>
inline constexpr void Logger::Warn(const FormatString& str, Args&&... item)
{
	Log(LogLevel::Warn, str, std::forward<Args>(item)...);
}

template<typename FormatString, typename... Args>
inline constexpr void Logger::Error(const FormatString& str, Args&&... item)
{
	Log(LogLevel::Error, str, std::forward<Args>(item)...);
}

template<typename FormatString, typename... Args>
inline constexpr void Logger::Critical(const FormatString& str, Args&&... item)
{
	Log(LogLevel::Critical, str, std::forward<Args>(item)...);
}

// Create formatter templates for unsigned char (opengl logging) and filesystem paths
template <>
struct std::formatter<unsigned char> : std::formatter<const char*> {
	auto format(const unsigned char* str, std::format_context& ctx) {
		return std::formatter<const char*>::format((const char*)str, ctx);
	}
};

template <>
struct std::formatter<std::filesystem::path> : std::formatter<std::string> {
	auto format(const std::filesystem::path& path, std::format_context& ctx) {
		return std::formatter<std::string>::format(path.string(), ctx);
	}
};