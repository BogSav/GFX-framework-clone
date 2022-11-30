#pragma once

#include "tema2/3dGeometries/GeometryObject.hpp"

#include "tema2/3dGeometries/3dPolygon.hpp"
#include "tema2/3dGeometries/3dPolyhedron.hpp"
#include "tema2/3dGeometries/3dTriangle.hpp"

class GameComponent
{
public:
	GameComponent() = delete;
	GameComponent(const Shader* const shader, CustomCamera* const camera)
		: m_shader(shader), m_camera(camera)
	{
	}

	virtual void Update(float) { return; }

	// Render with default color, shader and camera
	void Render() const
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this](const auto& current) { current->Render(m_modelMatrix); });
	}

	// Render with default color, shader and camera, but with car position for the curve and curve
	// coefficient
	void Render(const glm::vec3& carPosition, const float& coefficient) const
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this,
			 &carPosition = std::as_const(carPosition),
			 &coefficient = std::as_const(coefficient)](const auto& current)
			{ current->Render(m_modelMatrix, carPosition, coefficient); });
	}

	// Render with default color and shader, but with custom camera
	void Render(const CustomCamera* const camera) const
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this, &camera = std::as_const(camera)](const auto& current)
			{ current->Render(m_shader, camera, m_modelMatrix); });
	}

	// Render with default shader and camera, but with custom color
	void Render(const Color color) const
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this, &color = std::as_const(color)](const auto& current)
			{ current->Render(m_modelMatrix, color); });
	}

	// Render with default shader but custom color and camera
	void Render(const CustomCamera* const camera, const Color color) const
	{
		std::for_each(
			m_geometries.begin(),
			m_geometries.end(),
			[this, &color = std::as_const(color), &camera = std::as_const(camera)](
				const auto& current) { current->Render(m_shader, camera, m_modelMatrix, color); });
	}

protected:
	const Shader* const m_shader;
	CustomCamera* const m_camera;

	std::vector<std::unique_ptr<GeometryObject>> m_geometries;

	glm::mat4 m_modelMatrix = glm::mat4(1);
};