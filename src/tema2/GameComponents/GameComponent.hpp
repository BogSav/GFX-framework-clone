#pragma once

#include "tema2/Geometries/GeometryObject.hpp"
#include "tema2/Geometries/PlanarRectangle.hpp"
#include "tema2/Geometries/PlanarTriangle.hpp"

class GameComponent
{
public:
	GameComponent() = delete;
	GameComponent(
		Shader* shader,
		const CustomCamera* const camera)
		: m_shader(shader), m_camera(camera)
	{
	}

	virtual void Update(float) { return; }

	// Render with default color, shader and camera
	void Render()
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this](const auto& current) { current->Render(m_modelMatrix); });
	}

	// Render with default color and shader, but with custom camera
	void Render(const CustomCamera* const camera)
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this, &camera = std::as_const(camera)](const auto& current)
			{ current->Render(m_shader, camera, m_modelMatrix); });
	}

	// Render with default shader and camera, but with custom color
	void Render(const Color color)
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this, &color = std::as_const(color)](const auto& current)
			{ current->Render(m_modelMatrix, color); });
	}

	// Render with default shader but custom color and camera
	void Render(const CustomCamera* const camera, const Color color)
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this, &color, &camera = std::as_const(camera)](const auto& current)
			{ current->Render(m_shader, camera, m_modelMatrix, color); });
	}

protected:
	Shader* m_shader;
	const CustomCamera* m_camera;

	std::vector<std::unique_ptr<GeometryObject>> m_geometries;

	glm::mat4 m_modelMatrix = glm::mat4(1);
};