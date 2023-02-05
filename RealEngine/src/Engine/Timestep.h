#pragma once

#include <stdint.h>

class Timestep
{
public:
	Timestep(uint64_t time = 0.0) : m_Time(time) {}

	float GetMicroseconds() const { return (float)m_Time; }
	float GetMilliseconds() const { return (float)m_Time * 0.001f; }
	float GetSeconds() const { return (float)m_Time * 0.00001f; }
private:
	uint64_t m_Time;
};