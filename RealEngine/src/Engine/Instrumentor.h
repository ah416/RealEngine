#pragma once

#include <chrono>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <unordered_map>

#include <thread>

struct ProfileResult
{
	std::string Name;
	int64_t Start, End;
	uint32_t ThreadID;
};

struct InstrumentationSession
{
	std::string Name;
	std::ofstream m_OutputStream;
	int m_ProfileCount;
};

class Instrumentor
{
public:
	Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) {}

	void BeginSession(const std::string& name, const std::string& filepath = "results.json")
	{
		m_OutputStream.open(filepath);
		WriteHeader();
		m_CurrentSession = new InstrumentationSession{ name };
	}

	void EndSession()
	{
		WriteFooter();
		m_OutputStream.close();
		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;
		m_ProfileMap.clear();
	}

	void WriteProfile(const ProfileResult& result)
	{
		auto search = m_ProfileMap.find(result.Name);
		if (search != m_ProfileMap.end())
			search->second = (result.End - result.Start);
		else
			m_ProfileMap.insert({ result.Name, (result.End - result.Start) });

		if (m_ProfileCount++ > 0)
			m_OutputStream << ",";

		std::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"function\",";
		m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
		m_OutputStream << "\"name\":\"" << name << "\",";
		m_OutputStream << "\"ph\":\"X\",";
		m_OutputStream << "\"pid\":0,";
		m_OutputStream << "\"tid\":" << result.ThreadID << ",";
		m_OutputStream << "\"ts\":" << result.Start;
		m_OutputStream << "}";

		m_OutputStream.flush();
	}

	void WriteHeader()
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_OutputStream.flush();
	}

	void WriteFooter()
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	std::unordered_map<std::string, int64_t>& GetProfileMap() { return m_ProfileMap; }

	static Instrumentor& Get()
	{
		static Instrumentor i;
		return i;
	}
private:
	InstrumentationSession* m_CurrentSession;
	std::ofstream m_OutputStream;
	int m_ProfileCount;
	std::unordered_map<std::string, int64_t> m_ProfileMap;
};

class InstrumentationTimer
{
public:
	InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~InstrumentationTimer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		int64_t start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		int64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });
	}
private:
	const char* m_Name;
	bool m_Stopped;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};