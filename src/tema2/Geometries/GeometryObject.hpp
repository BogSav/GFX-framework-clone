#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "tema2/Utilities/Colors.hpp"
#include "tema2/Utilities/Camera.hpp"

#include "tema2/Utilities/Transformations.hpp"


class GeometryObject
{
public:
	GeometryObject() = default;
	GeometryObject(const Shader* const shader, CustomCamera* const camera)
		: m_shader(shader), m_camera(camera)
	{
	}

	void Render(const glm::mat4& modelMatrix) const
	{
		this->SendDataToShader(modelMatrix);

		m_mesh->Render();
	}

	void Render(const glm::mat4& modelMatrix, const glm::vec3& carPosition, const float& coefficient) const
	{
		this->SendDataToShader(modelMatrix);

		int location = glGetUniformLocation(m_shader->program, "CarPosition");
		glUniform3fv(location, 1, glm::value_ptr(carPosition));

		location = glGetUniformLocation(m_shader->program, "CurveCoefficient");
		glUniform1f(location, coefficient);

		m_mesh->Render();
	}

	void Render(const glm::mat4& modelMatrix, const Color& color) const
	{
		this->SendDataToShader(modelMatrix);

		int location = glGetUniformLocation(m_shader->program, "UniformColor");
		glUniform3fv(location, 1, glm::value_ptr(color()));

		m_mesh->Render();
	}

	void Render(
		const Shader* const shader, const CustomCamera* const camera, const glm::mat4& modelMatrix) const
	{
		if (!m_mesh || !shader || !shader->program)
			return;

		shader->Use();

		glUniformMatrix4fv(
			shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

		glUniformMatrix4fv(
			shader->loc_projection_matrix,
			1,
			GL_FALSE,
			glm::value_ptr(camera->GetProjectionMatrix()));

		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		m_mesh->Render();
	}

	void Render(
		const Shader* const shader,
		const CustomCamera* const camera,
		const glm::mat4& modelMatrix,
		const Color color) const
	{
		if (!m_mesh || !shader || !shader->program)
			return;

		shader->Use();

		glUniformMatrix4fv(
			shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

		glUniformMatrix4fv(
			shader->loc_projection_matrix,
			1,
			GL_FALSE,
			glm::value_ptr(camera->GetProjectionMatrix()));

		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		int location = glGetUniformLocation(shader->program, "UniformColor");
		glUniform3fv(location, 1, glm::value_ptr(color()));

		m_mesh->Render();
	}

private:
	void SendDataToShader(const glm::mat4& modelMatrix) const
	{
		assert(m_shader != nullptr);
		assert(m_camera != nullptr);

		if (!m_mesh || !m_shader || !m_shader->program)
			return;

		m_shader->Use();

		glUniformMatrix4fv(
			m_shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetViewMatrix()));

		glUniformMatrix4fv(
			m_shader->loc_projection_matrix,
			1,
			GL_FALSE,
			glm::value_ptr(m_camera->GetProjectionMatrix()));

		glUniformMatrix4fv(m_shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	}

protected:
	std::unique_ptr<Mesh> m_mesh = nullptr;

	const Shader* m_shader = nullptr;
	CustomCamera* m_camera = nullptr;
};