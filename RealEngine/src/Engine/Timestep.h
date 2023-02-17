#pragma once

#include <stdint.h>

class Timestep
{
public:
	Timestep(long double time = 0.0) : m_Time(time) {}

	// Not properly implemented
	float GetMicroseconds() const { return (float)m_Time * 1000; }

	float GetMilliseconds() const { return (float)m_Time; }
	
	float GetSeconds() const { return (float)m_Time * 0.0001; }
private:
	long double m_Time;
};