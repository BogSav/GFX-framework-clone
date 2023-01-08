#pragma once

#include "tema3/3dGeometries/3dPolygon.hpp"
#include "tema3/3dGeometries/3dPolyhedron.hpp"
#include "tema3/3dGeometries/3dTriangle.hpp"
#include "tema3/Lighting/LightSource.hpp"

class GameComponent
{
protected:
	GameComponent() = default;

public:
	// Update function for each individual component
	virtual void Update(float) { return; }

	// Render with default color, shader and camera
	virtual void Render() { return; };

protected:
	glm::mat4 m_modelMatrix = glm::mat4(1);
};