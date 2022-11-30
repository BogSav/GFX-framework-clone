#pragma once

#include "tema2/3dGeometries/GeometryObject.hpp"

class LightingComponent
{
public:
	LightingComponent() = default;

	const glm::vec3& GetPosition() { return m_position; }

private:
	glm::vec3 m_position;

	static constexpr unsigned int materialShiness = 30;
	
	static constexpr float materialKd = 0.5;
	static constexpr float materialKs = 0.5;
};