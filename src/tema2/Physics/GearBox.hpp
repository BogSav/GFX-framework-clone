#pragma once

#include "tema2/Utilities/Timer.hpp"

#include <assert.h>
#include <vector>

struct GearBox
{
public:
	void Reset()
	{
		currentGear = 1;
		isInShift = false;
	}

	double GetNeededSpeedToShiftGearDown() const { return speeds[currentGear - 1]; }
	double GetNeededSpeedToShiftGearUp() { return speeds[currentGear]; }
	double GetForceForCurrentGear()
	{
		if (isInShift)
		{
			if (m_timer.PassedTime(0.4))
			{
				isInShift = false;
			}
			return 0;
		}
		return forces[currentGear];
	}
	void ShiftGearUp()
	{
		assert(currentGear < 6);
		currentGear++;
		isInShift = true;
	};
	void ShiftGearDown()
	{
		assert(currentGear > 0);
		currentGear--;
	};
	int GetCurrentGear() const { return currentGear; }
	double GetBrakeForce() const { return brakeFroce; }

private:
	std::vector<double> forces = {0, 40000, 35000, 30000, 25000, 20000, 15000};  // Newtons
	std::vector<double> speeds = {0, 20, 40, 60, 80, 100, 180};  // m/s
	int currentGear = 1;
	double brakeFroce = 30000;

	DTimer m_timer;
	bool isInShift = false;
};