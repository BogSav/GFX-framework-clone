#include "Masina.hpp"

Masina::Masina(const WindowObject* window, Shader* shader) : m_shader(shader)
{
	m_mesh = std::make_unique<Mesh>();
	m_mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "mele"), "F1.obj");

	m_camera = new CustomCamera();
	m_camera->Set(
		glm::vec3(0, 2, 3.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), window->props.aspectRatio);

	m_modelMatrix = glm::mat4(1);
}

void Masina::Render()
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

	glUniformMatrix4fv(m_shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

	m_mesh->Render();
}