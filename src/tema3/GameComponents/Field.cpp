#include "Field.hpp"

#include <iostream>
#include <numbers>

Field::Field(
	const Shader* const shader,
	std::shared_ptr<CustomCamera>& camera,
	std::shared_ptr<Texture2D>& texture)
	: GameComponent(), m_inclinationAngle(RADIANS(30))
{
	m_plane = std::make_unique<Polygon3d>(
		shader,
		camera.get(),
		glm::vec3{-0.5, 0, -0.5},
		glm::vec3{-0.5, 0, 0.5},
		glm::vec3{0.5, 0, 0.5},
		glm::vec3{0.5, 0, -0.5},
		Colors::Cyan);
	m_plane->ComputeInitialBBox();

	m_scale = 50.f;

	m_position = glm::vec3{0, 0, 0};
	m_movingDirection = glm::vec3{0, -glm::sin(m_inclinationAngle), glm::cos(m_inclinationAngle)};
	m_movingSpeed = 5.f;

	m_textureDirection = glm::vec2{0, 1};
	m_texturePosition = glm::vec2{0, 0};
	m_textureSpeed = 0.1f;

	m_directionAngleWithOX = 0;

	m_texture = texture;
	m_camera = camera;
}

void Field::Render()
{
	m_modelMatrix = glm::mat4(1);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, m_inclinationAngle, glm::vec3{1.f, 0, 0});
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{m_scale, 1.f, m_scale});

	// m_plane->Render(modelMatrix, m_texture.get());
	m_plane->Render(m_modelMatrix, m_texture.get(), m_texturePosition);
	m_bbox = m_plane->GetTranformedBBox(m_modelMatrix);
}

void Field::Update(float deltaTime)
{
	m_position += m_movingDirection * m_movingSpeed * deltaTime;

	m_camera->MoveInTheSpecifiedDirection(m_movingDirection, m_movingSpeed * deltaTime);

	m_texturePosition += m_textureDirection * deltaTime * m_textureSpeed;
	if (m_texturePosition.x >= 1 || m_texturePosition.x <= -1)
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

		m_movingDirection.x = (2.f * mouseX / resolution.x - 1.f) * 3.f;

		//mousePosition.y *= -1.f;
		//m_textureDirection.x = 1;
		m_textureDirection.y = 1.f;
		m_textureDirection.x = m_movingDirection.x;
	}
}
