#pragma once

#include "GameComponent.hpp"
#include "Player.hpp"

class Field : public GameComponent
{
public:
	Field() = delete;
	Field(const Shader* const shader, CustomCamera* const camera, std::string texturePath);

	void Render() override;
	//void Render(const Shader* shader, CustomCamera* camera);

	void Update(float) override;
	void UpdateTextureDirection(int mouseX, int mouseY, glm::ivec2);

	friend Player;

private:
	std::unique_ptr<GeometryObject3d> m_plane;
	std::unique_ptr<Texture2D> m_texture;

	std::unique_ptr<Mesh> testMesh;

	float m_scale;
	float m_movingSpeed;

	glm::vec3 m_movingDirection;
	glm::vec3 m_currentPosition;

	glm::vec2 m_textureDirection;
	glm::vec2 m_texturePosition;
	float m_textureSpeed;

	float m_directionAngleWithOX = 0.f;
	float m_inclinationAngle;
};