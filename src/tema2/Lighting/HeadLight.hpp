#pragma once

#include "LightSource.hpp"

class HeadLight : public SpotLight
{
public:
	HeadLight() = delete;
	HeadLight(
		const glm::vec3& position,
		const Color& color,
		const float& intensity,
		const float& cutOff,
		const glm::vec3& direction,
		const glm::vec3& right)
		:	SpotLight(position, color, intensity, cutOff, direction, right)
	{
	}
};