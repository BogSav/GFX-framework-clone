#pragma once

#include "Geometry/Utilities.hpp"
#include "utils/glm_utils.h"

typedef glm::vec3 Color;

struct Colors
{
	static glm::vec4 getColorPlusAlpha(Color color, float alpha)
	{
		return {color[0], color[1], color[2], alpha};
	}

	static constexpr glm::vec3 WHITE = {1, 1, 1};
	static constexpr glm::vec3 BLACK = {0, 0, 0};

	static constexpr glm::vec3 RED = {1, 0, 0};
	static constexpr glm::vec3 GREEN = {0, 1, 0};
	static constexpr glm::vec3 BLUE = {0, 0, 1};

	static constexpr glm::vec3 PINK = {238 / 255, 16 / 255, 215 / 255};
	static constexpr glm::vec3 YELLOW = {1, 1, 0};
	static constexpr glm::vec3 CYAN = {0, 1, 0.9};

	static constexpr glm::vec3 DARK_RED = {0.5, 0.21, 0.21};

	static constexpr glm::vec3 GRAY = {0.32, 0.32, 0.32};
	static constexpr glm::vec3 LIGHT_GRAY = {0.75, 0.75, 0.75};

	static constexpr glm::vec3 LIGHT_GOLD = {0.93, 0.82, 0.56};
	static constexpr glm::vec3 DARK_GOLD = {0.62, 0.4, 0.22};
};