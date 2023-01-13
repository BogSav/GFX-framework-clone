#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "ColllsionEngine.hpp"

class BoundingBox
{
public:
	BoundingBox() = default;

	void InitBBoxBasedOnMesh(const Mesh* mesh);
	void Reset();

	BoundingBox& operator+=(const BoundingBox& rhs);
	friend BoundingBox operator+(const BoundingBox& lhs, const BoundingBox& rhs);
	friend std::ostream& operator<<(std::ostream&, BoundingBox&);

	glm::vec3 GetMinCorner() const { return m_minCorner; }
	glm::vec3 GetMaxCorner() const { return m_maxCorner; }

	glm::vec3 GetCenterPoint() const;

	BoundingBox GetTranformedBBox(glm::mat4& modelMatrix);

	friend CollisionEngine;

private:
	void RearrangeCorners();

private:
	glm::vec3 m_minCorner = glm::vec3{0, 0, 0};
	glm::vec3 m_maxCorner = glm::vec3{0, 0, 0};
};