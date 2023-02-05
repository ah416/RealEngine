#pragma once

//#define LOG_ENABLE_SOURCE_LOC // enables source location tracking automatically in the logger
#include "Engine/Tools/Logger.h"

#include <memory>

// Defines and includes for Windows
#ifdef _WIN32

// Including Windows.h and using GLM without these defs causes errors
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

#endif

// DEBUG
#ifdef REAL_DEBUG

// Debug assert uses __debugbreak() which causes the MSVC debugger to enter break mode, release does not use this
#define REAL_ASSERT(x, text) if (!x) { ::Logger::Get().Error(text); __debugbreak(); }

// Only prints in debug
#define REAL_DEBUG(...) ::Logger::Get().Info(__VA_ARGS__)

#define REAL_INFO(...) ::Logger::Get().Info(__VA_ARGS__)

#define REAL_WARN(...) ::Logger::Get().Warn(__VA_ARGS__)

#define REAL_ERROR(...) ::Logger::Get().Error(__VA_ARGS__)

#define REAL_CRITICAL(...) ::Logger::Get().Critical(__VA_ARGS__)

// Define this to allow profiling
#define PROFILE

// Define this to diplay profile results in the custom imgui debug window (OpenGL/ImGuiPrefabs/DebugWindow.h) 
#define PROFILE_IMGUI

#else // RELEASE //

#define REAL_ASSERT(x, text, ...) if (!x) { ::Logger::Get().Error(text); __VA_ARGS__ }

#define REAL_DEBUG(...)

#define REAL_INFO(...) ::Logger::Get().Info(__VA_ARGS__)

#define REAL_WARN(...) ::Logger::Get().Warn(__VA_ARGS__)

#define REAL_ERROR(...) ::Logger::Get().Error(__VA_ARGS__)

#define REAL_CRITICAL(...) ::Logger::Get().Critical(__VA_ARGS__)

#endif

#ifdef PROFILE
#include "Engine/Instrumentor.h"

#define PROFILE_BEGIN_SESSION(name, filepath) Instrumentor::Get().BeginSession(name, filepath)
#define PROFILE_END_SESSION() Instrumentor::Get().EndSession()
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)

#else // NO PROFILING //

#define PROFILE_BEGIN_SESSION(name, filepath)
#define PROFILE_END_SESSION()
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()

#endif

// A different name for shared_ptr, easier to type
template<typename T>
using Ref = std::shared_ptr<T>;