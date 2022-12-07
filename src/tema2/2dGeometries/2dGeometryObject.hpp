#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "tema2/Utilities/Colors.hpp"
#include "tema2/Utilities/Utilities.hpp"

#include <numbers>

class GeometryObject2d
{
public:
	GeometryObject2d() = default;

	void Render(
		const Shader* const shader, const glm::mat3& transformMatrix, const glm::mat3& modelMatrix)
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

protected:
	std::unique_ptr<Mesh> m_mesh = nullptr;
};