#pragma once

#include "tema2/Geometries/GeometryObject.hpp"

class GameComponent
{
public:
	GameComponent() = delete;
	GameComponent(
		Shader* shader,
		const gfxc::Camera* const camera,
		const glm::vec3& origin = glm::vec3{0, 0, 0},
		const double scale = 1.)
		: m_shader(shader), m_camera(camera), m_origin(origin), m_scale(scale)
	{
	}

	void Render()
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this](const auto& current) { current->Render(m_modelMatrix); });
	}

	void Render(const Color color)
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this, &color](const auto& current) { current->Render(m_modelMatrix, color); });
	}

protected:
	Shader* m_shader;
	const gfxc::Camera* m_camera;

	std::vector<std::unique_ptr<GeometryObject>> m_geometries;

	glm::mat4 m_modelMatrix = glm::mat4(1);
	glm::vec3 m_origin;
	double m_scale;
};