#pragma once

#include <chrono>

class Timer
{
public:
	Timer() = default;
	bool TimePassedValue(float duration) { return this->GetTimeElaspsed() > duration; }
	float GetTimeElaspsed()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(
				   std::chrono::steady_clock::now() - m_startPoint)
			.count() / 1000.f;
	}
	void ResetTime() { m_startPoint = std::chrono::steady_clock::now(); }
private:
	std::chrono::steady_clock::time_point m_startPoint;
};