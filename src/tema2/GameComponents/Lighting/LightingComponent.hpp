#pragma once

#include "tema2/3dGeometries/GeometryObject.hpp"

class LightingComponent
{
public:
	LightingComponent() = default;

	friend class GeometryObject;

protected:
	virtual const glm::vec3& operator()() const = 0;
	virtual const glm::vec3& GetLightSourcePosition() const = 0;

protected:
	glm::vec3 m_position;

private:
	static constexpr unsigned int materialShiness = 30;

	static constexpr float materialKd = 0.5;
	static constexpr float materialKs = 0.5;
};