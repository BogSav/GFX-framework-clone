#pragma once

#include "GeometryObject.hpp"

class PlanarTriangle : public GeometryObject
{
public:
	PlanarTriangle(
		glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color, const bool wireframe = false)
	{
		InitMesh(v1, v2, v3, color, wireframe);
	}

	PlanarTriangle(
		Shader* shader,
		const gfxc::Camera* const camera,
		glm::vec3 v1,
		glm::vec3 v2,
		glm::vec3 v3,
		glm::vec3 color,
		const bool wireframe = false)
		: GeometryObject(shader, camera)
	{
		InitMesh(v1, v2, v3, color, wireframe);
	}

private:
	void InitMesh(
		glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color, const bool wireframe)
	{
		m_vertices.reserve(3);
		m_vertices.emplace_back(v1, color);
		m_vertices.emplace_back(v2, color);
		m_vertices.emplace_back(v3, color);

		m_vertices.reserve(3);
		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);

		m_mesh = std::make_unique<Mesh>();

		if (wireframe)
		{
			m_mesh->SetDrawMode(GL_LINE_LOOP);
		}

		m_mesh->InitFromData(m_vertices, m_indices);
	}
};