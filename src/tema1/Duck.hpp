#pragma once

#include "Object.hpp"
#include "BoundingBox.hpp"

#include "Triangle.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"

#include "utils/gl_utils.h"

#include "2dTransformations.hpp"

class Duck
{
public:
	Duck() = delete;
	Duck(glm::vec3 position, float speed, TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace viewPort);

	void Update(float deltaTime);

	void Render(Shader* shader, const gfxc::Camera* const camera);

	BoundingBox GetBoundingBox() const;
private:
	void CalculateBoundingBox();

private:
	BoundingBox m_bbox;
	std::unordered_map<std::string, Object*> m_components;

	glm::mat3 m_duckModelMatrix = glm::mat3(1);

	glm::mat3 m_rightArmModelMatrix = glm::mat3(1);
	glm::mat3 m_leftArmModelMatrix = glm::mat3(1);

	glm::vec2 m_position;
	glm::vec2 m_flyingDirection;
	float m_flyingSpeed;

	TranformUtils::LogicSpace m_logicSpace;
	TranformUtils::ViewportSpace m_viewPort;
};