#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "tema2/Utilities/Colors.hpp"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

class Polygon2d
{
public:
	Polygon2d() = delete;
	Polygon2d(
		const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec2& v4, Color color)
	{
		std::vector<VertexFormat> vertices;
		vertices.reserve(4);
		vertices.emplace_back(glm::vec3{v1[0], v1[1], 1}, color());
		vertices.emplace_back(glm::vec3{v2[0], v2[1], 1}, color());
		vertices.emplace_back(glm::vec3{v3[0], v3[1], 1}, color());
		vertices.emplace_back(glm::vec3{v4[0], v4[1], 1}, color());

		std::vector<unsigned int> indices;
		indices.reserve(6);
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);
		indices.push_back(2);

		m_mesh = std::make_unique<Mesh>();

		m_mesh->InitFromData(vertices, indices);
	}

	Polygon2d(const glm::vec2& v1, const double& width, const double& height, Color color)
		: Polygon2d(
			v1,
			v1 + glm::vec2{0, height},
			v1 + glm::vec2{width, height},
			v1 + glm::vec2{width, 0},
			color)
	{
	}

	void Render(const Shader* const shader, const glm::mat3& transformMatrix, const glm::mat3& modelMatrix)
	{
		if (!m_mesh || !shader || !shader->program)
			return;

		shader->Use();

		int position = glGetUniformLocation(shader->program, "TransformMatrix");
		glUniformMatrix3fv(position, 1, GL_FALSE, glm::value_ptr(transformMatrix));

		position = glGetUniformLocation(shader->program, "Model");
		glUniformMatrix3fv(position, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		m_mesh->Render();
	}

private:
	std::unique_ptr<Mesh> m_mesh;
};