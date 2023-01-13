#include "Player.hpp"

#include "Field.hpp"

#include <iostream>

Player::Player(const Shader* const shader, CustomCamera* const camera, const Field* field)
	: m_field(field), GameComponent()
{
	m_cube = std::make_unique<Polyhedron3d>(
		shader, camera, glm::vec3{-0.5f, -0.5f, -0.5f}, 1.f, 1.f, 1.f, Colors::Red);
	m_rightSki = std::make_unique<Polyhedron3d>(
		shader, camera, glm::vec3{-1.f, -0.05f, -0.15f}, 2.f, 0.3f, 0.1f, Colors::Cyan);
	m_leftSki = std::make_unique<Polyhedron3d>(
		shader, camera, glm::vec3{-1.f, -0.05f, -0.15f}, 2.f, 0.3f, 0.1f, Colors::Cyan);

	m_cube->ComputeInitialBBox();
	m_rightSki->ComputeInitialBBox();
	m_leftSki->ComputeInitialBBox();

	m_scale = 1.f;
}

void Player::Render()
{
	m_modelMatrix = glm::mat4(1);
	m_modelMatrix = glm::translate(m_modelMatrix, m_field->m_position + glm::vec3{0, 0.5, 0.5});
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{m_scale, m_scale, m_scale});
	m_modelMatrix = glm::rotate(m_modelMatrix, m_field->m_inclinationAngle, glm::vec3{1, 0, 0});
	m_modelMatrix = glm::rotate(m_modelMatrix, m_field->m_directionAngleWithOX, glm::vec3{0, 1, 0});

	m_cube->Render(m_modelMatrix);
	m_bbox = m_cube->GetTranformedBBox(m_modelMatrix);

	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{1.2, 1, 1});

	glm::mat4 rightSkiModelMatrix = glm::translate(m_modelMatrix, glm::vec3{0, -0.5, 0.5});
	m_rightSki->Render(rightSkiModelMatrix);
	m_bbox += m_rightSki->GetTranformedBBox(rightSkiModelMatrix);

	glm::mat4 leftSkiModelMatrix = glm::translate(m_modelMatrix, glm::vec3{0, -0.5, -0.5});
	m_leftSki->Render(leftSkiModelMatrix);
	m_bbox += m_leftSki->GetTranformedBBox(leftSkiModelMatrix);
}
