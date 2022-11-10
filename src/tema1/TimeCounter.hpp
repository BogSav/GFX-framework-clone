#pragma once

#include <chrono>

class Timer
{
public:
	bool PassedTime(float durration) {

	}
	void ResetStartPoint() {

	}

private:
	std::chrono::time_point<float> m_startPoint;
};