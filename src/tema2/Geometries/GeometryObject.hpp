#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"

class GeometryObject
{
public:
	GeometryObject() = default;
	GeometryObject(Shader* shader, const gfxc::Camera* const camera)
		: m_shader(shader), m_camera(camera)
	{
	}

	void Render(const glm::mat4& modelMatrix) const
	{
		this->UseShader(modelMatrix);

		m_mesh->Render();
	}

	void Render(const glm::mat4& modelMatrix, const glm::vec3& color) 
	{
		this->UseShader(modelMatrix);

		int location = glGetUniformLocation(m_shader->program, "UniformColor");
		glUniform3fv(location, 1, glm::value_ptr(color));

		m_mesh->Render();
	}

	void RenderByCustomShaderAndCamera(
		Shader* shader, const gfxc::Camera* const camera, const glm::mat4& modelMatrix) const
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

	void ChangeShader(Shader* newShader)
	{
		m_shader = nullptr;
		m_shader = newShader;
	}

private:
	void UseShader(const glm::mat4& modelMatrix) const
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

		m_mesh->Render();
	}

protected:
	std::unique_ptr<Mesh> m_mesh = nullptr;
	std::vector<VertexFormat> m_vertices;
	std::vector<unsigned int> m_indices;

	Shader* m_shader = nullptr;
	const gfxc::Camera* m_camera = nullptr;
};