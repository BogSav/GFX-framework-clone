#pragma once

#include "GameComponent.hpp"

class Field : public GameComponent
{
public:
	Field() = delete;
	Field(
		Shader* shader,
		const CustomCamera* const camera,
		const glm::vec3 startPos,
		const float width,
		const float length)
		: GameComponent(shader, camera), m_startPosition(startPos), m_width(width), m_length(length)
	{
		m_geometries.emplace_back(new PlanarRectangle(
			m_shader,
			m_camera,
			startPos + glm::vec3{0, -0.1, 0},
			startPos + glm::vec3{m_width, -0.1, 0},
			startPos + glm::vec3{m_width, -0.1, m_length},
			startPos + glm::vec3{0, -0.1, m_length},
			Colors::DarkGreen));
	}

	glm::vec3 Getposition() const { return m_startPosition; }
	float GetWidth() const { return m_width; }
	float GetLength() const { return m_length; }

	private:
	glm::vec3 m_startPosition;
	float m_width;
	float m_length;
};