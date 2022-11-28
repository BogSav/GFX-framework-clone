#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "tema2/Utilities/Colors.hpp"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include <numbers>

class Circle2d
{
public:
	Circle2d() = delete;
	Circle2d(const glm::vec2 origin, const float radius, const int nrOfSteps, Color color)
	{
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;

		vertices.emplace_back(glm::vec3{origin.x, origin.y, 1}, color());
		vertices.emplace_back(glm::vec3{origin.x, origin.y, 1} + glm::vec3{radius, 0, 0}, color());

		float angularStep = std::numbers::pi_v<float> / nrOfSteps;
		int nrOfTriangles = nrOfSteps * 4 - 1;
		int trNr = 0;

		for (float currentAngle = 0; trNr < nrOfTriangles; currentAngle += angularStep, trNr++)
		{
			vertices.emplace_back(
				glm::vec3{origin.x, origin.y, 1}
					+ glm::vec3{sinf(currentAngle), cosf(currentAngle), 0} * radius,
				color());

			indices.push_back(trNr + 1);
			indices.push_back(0);
			indices.push_back(trNr + 2);
		}

		indices.push_back(1);
		indices.push_back(0);
		indices.push_back(nrOfTriangles + 1);

		m_mesh = std::make_unique<Mesh>();

		m_mesh->InitFromData(vertices, indices);
	}

	void Render(const Shader* shader, const glm::mat3& transformMatrix, const glm::mat3& modelMatrix)
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