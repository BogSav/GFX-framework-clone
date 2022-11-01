#pragma once

#include "utils/glm_utils.h"

struct Colors
{
	static constexpr glm::vec3 WHITE = { 1, 1, 1 };
	static constexpr glm::vec3 BLACK = { 0, 0, 0 };

	static constexpr glm::vec3 RED = {1, 0, 0};
	static constexpr glm::vec3 GREEN = {0, 1, 0};
	static constexpr glm::vec3 BLUE = { 0, 0, 1 };
	
	static constexpr glm::vec3 PINK = { 238 / 255, 16 / 255, 215 / 255 };
	static constexpr glm::vec3 YELLOW = { 1, 1, 0 };
	static constexpr glm::vec3 CYAN = { 0, 1, 230 / 255 };
};