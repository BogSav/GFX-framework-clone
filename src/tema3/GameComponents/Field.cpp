#include "Field.hpp"

#include <iostream>
#include <numbers>

Field::Field(const Shader* const shader, CustomCamera* const camera, std::string texturePath)
	: GameComponent()
{
	m_plane = std::make_unique<Polygon3d>(
		shader,
		camera,
		glm::vec3{-0.5, 0, -0.5},
		glm::vec3{-0.5, 0, 0.5},
		glm::vec3{0.5, 0, 0.5},
		glm::vec3{0.5, 0, -0.5},
		Colors::Cyan);

	m_scale = 50.f;

	m_currentPosition = glm::vec3{0, 0, 0};
	m_movingDirection = glm::vec3{0, -0.7f, 0.7f};
	m_movingSpeed = 2.f;

	m_textureDirection = glm::vec2{0, 1};
	m_texturePosition = glm::vec2{0, 0};
	m_textureSpeed = 0.1f;

	m_directionAngleWithOX = 0;

	m_texture = std::make_unique<Texture2D>();
	m_texture->Load2D(texturePath.c_str(), GL_REPEAT);
}

void Field::Render()
{
	m_modelMatrix = glm::mat4(1);
	m_modelMatrix = glm::translate(m_modelMatrix, m_currentPosition);
	m_modelMatrix = glm::rotate(m_modelMatrix, RADIANS(30), glm::vec3{1.f, 0, 0});
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{m_scale, 1.f, m_scale});

	// m_plane->Render(modelMatrix, m_texture.get());
	m_plane->Render(m_modelMatrix, m_texture.get(), m_texturePosition);
}

void Field::Update(float deltaTime)
{
	m_currentPosition += m_movingDirection * m_movingSpeed * deltaTime;

	m_texturePosition += m_textureDirection * deltaTime * m_textureSpeed;
	if (m_texturePosition.x >= 1)
		m_texturePosition.x = 0;
	if (m_texturePosition.y >= 1)
		m_texturePosition.y = 0;

}

void Field::UpdateTextureDirection(int mouseX, int mouseY, glm::ivec2 resolution)
{
	mouseY = resolution.y - mouseY;

	glm::vec2 mousePosition = glm::vec2{mouseX - resolution.x / 2, mouseY - resolution.y / 2};
	float angle = glm::atan2(mousePosition.y, mousePosition.x);
	if (angle < 0)
		angle += 2 * std::numbers::pi_v<float>;

	if (RADIANS(195) < angle && angle < RADIANS(345))
	{
		m_directionAngleWithOX = angle;

		mousePosition.y *= -1.f;
		m_textureDirection = glm::normalize(mousePosition);
	}
}
