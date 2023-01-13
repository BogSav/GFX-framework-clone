#pragma once

#include "GameComponent.hpp"
#include "Player.hpp"

class Field : public GameComponent
{
public:
	Field() = delete;
	Field(
		const Shader* const shader,
		std::shared_ptr<CustomCamera>& camera,
		std::shared_ptr<Texture2D>& texture);

	void Render() override;
	//void Render(const Shader* shader, CustomCamera* camera);

	void Update(float) override;
	void UpdateTextureDirection(int mouseX, int mouseY, glm::ivec2);

	friend Player;

private:
	std::unique_ptr<GeometryObject3d> m_plane;
	std::shared_ptr<Texture2D> m_texture;
	std::shared_ptr<CustomCamera>  m_camera;

	float m_movingSpeed;

	glm::vec3 m_movingDirection;

	glm::vec2 m_textureDirection;
	glm::vec2 m_texturePosition;
	float m_textureSpeed;

	float m_directionAngleWithOX = 0.f;
	const float m_inclinationAngle;
};