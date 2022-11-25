#pragma once

#include "GeometryObject.hpp"

class PlanarTriangle : public GeometryObject
{
public:
	PlanarTriangle(
		const glm::vec3 v1,
		const glm::vec3 v2,
		const glm::vec3 v3,
		const Color color,
		const bool wireframe = false)
	{
		InitMesh(v1, v2, v3, color, wireframe);
	}

	PlanarTriangle(
		Shader* shader,
		const CustomCamera* const camera,
		const glm::vec3 v1,
		const glm::vec3 v2,
		const glm::vec3 v3,
		const Color color,
		const bool wireframe = false)
		: GeometryObject(shader, camera)
	{
		InitMesh(v1, v2, v3, color, wireframe);
	}

private:
	void InitMesh(
		const glm::vec3 v1,
		const glm::vec3 v2,
		const glm::vec3 v3,
		const Color color,
		const bool wireframe)
	{
		std::vector<VertexFormat> vertices;
		vertices.reserve(3);
		vertices.emplace_back(v1, color());
		vertices.emplace_back(v2, color());
		vertices.emplace_back(v3, color());

		std::vector<unsigned int> indices;
		indices.reserve(3);
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		m_mesh = std::make_unique<Mesh>();

		if (wireframe)
		{
			m_mesh->SetDrawMode(GL_LINE_LOOP);
		}

		m_mesh->InitFromData(vertices, indices);
	}
};