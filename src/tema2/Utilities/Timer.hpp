#pragma once

#include <chrono>

template<class T>
class Timer
{
public:
	Timer() = default;

	bool PassedTime(const T duration)
	{
		if (!m_isActive)
		{
			m_isActive = true;
			ResetTime();
			return false;
		}

		if(TimePassedValue(duration))
		{
			m_isActive = false;
			return true;
		}

		return false;
	}

	bool TimePassedValue(T duration) { return this->GetTimeElaspsed() > duration; }

	T GetTimeElaspsed()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(
				   std::chrono::steady_clock::now() - m_startPoint)
				   .count()
			/ (T)1000;
	}

	void ResetTime() { m_startPoint = std::chrono::steady_clock::now(); }
	void ResetTimer()
	{
		m_isActive = false;
		ResetTime();
	}

private:
	std::chrono::steady_clock::time_point m_startPoint = std::chrono::steady_clock::now();
	bool m_isActive = false;
};

typedef Timer<float> FTimer;
typedef Timer<double> DTimer;