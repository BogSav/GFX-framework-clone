#include "GeometryObject.hpp"

#include "tema2/GameComponents/Lighting/LightingComponent.hpp"
#include "tema2/GameComponents/Lighting/StreetLight.hpp"

GeometryObject::GeometryObject(const Shader* const shader, CustomCamera* const camera)
	: m_shader(shader), m_camera(camera)
{
}

void GeometryObject::Render(const glm::mat4& modelMatrix) const
{
	this->SendDataToShader(modelMatrix);

	m_mesh->Render();
}

void GeometryObject::Render(
	const glm::mat4& modelMatrix, const glm::vec3& carPosition, const float& coefficient) const
{
	this->SendDataToShader(modelMatrix);

	int location = glGetUniformLocation(m_shader->program, "CarPosition");
	glUniform3fv(location, 1, glm::value_ptr(carPosition));

	location = glGetUniformLocation(m_shader->program, "CurveCoefficient");
	glUniform1f(location, coefficient);

	m_mesh->Render();
}

void GeometryObject::Render(
	const glm::mat4& modelMatrix,
	const glm::vec3& carPosition,
	const glm::vec3& eyePosition,
	const float& coefficient,
	const std::vector<const LightingComponent*>& lightingComponents) const
{
	this->SendDataToShader(modelMatrix);

	// Send data for curve calculation
	int location = glGetUniformLocation(m_shader->program, "CarPosition");
	glUniform3fv(location, 1, glm::value_ptr(carPosition));
	location = glGetUniformLocation(m_shader->program, "CurveCoefficient");
	glUniform1f(location, coefficient);

	// Send the position of the camera
	location = glGetUniformLocation(m_shader->program, "eye_position");
	glUniform3fv(location, 1, glm::value_ptr(eyePosition));

	// Send material properties
	location = glGetUniformLocation(m_shader->program, "material_kd");
	glUniform1f(location, LightingComponent::materialKd);
	location = glGetUniformLocation(m_shader->program, "material_ks");
	glUniform1f(location, LightingComponent::materialKs);
	location = glGetUniformLocation(m_shader->program, "material_shininess");
	glUniform1i(location, LightingComponent::materialShiness);

	// Send position of the sources
	location = glGetUniformLocation(m_shader->program, "nrOfLightSources");
	glUniform1i(location, static_cast<int>(lightingComponents.size()));

	// Compute the positions
	std::vector<glm::vec3> lightingPositions;
	lightingPositions.reserve(lightingComponents.size());
	for (const LightingComponent* current : lightingComponents)
	{
		glm::vec3 arr = (*current)();
		lightingPositions.push_back(arr);
	}

	// Send the positions
	location = glGetUniformLocation(m_shader->program, "light_positions");
	glUniform3fv(
		location,
		static_cast<GLint>(lightingPositions.size()),
		reinterpret_cast<GLfloat*>(lightingPositions.data()));

	m_mesh->Render();
}

void GeometryObject::Render(const glm::mat4& modelMatrix, const Color& color) const
{
	this->SendDataToShader(modelMatrix);

	int location = glGetUniformLocation(m_shader->program, "UniformColor");
	glUniform3fv(location, 1, glm::value_ptr(color()));

	m_mesh->Render();
}

void GeometryObject::Render(
	const Shader* const shader,
	const CustomCamera* const camera,
	const glm::mat4& modelMatrix) const
{
	if (!m_mesh || !shader || !shader->program)
		return;

	shader->Use();

	glUniformMatrix4fv(
		shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	glUniformMatrix4fv(
		shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	m_mesh->Render();
}

void GeometryObject::Render(
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
		shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int location = glGetUniformLocation(shader->program, "UniformColor");
	glUniform3fv(location, 1, glm::value_ptr(color()));

	m_mesh->Render();
}

void GeometryObject::SendDataToShader(const glm::mat4& modelMatrix) const
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

void GeometryObject::CalculateVerticesNormals(
	std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
	for (size_t i = 0; i < indices.size() - 2; i += 3)
	{
		vertices[indices[i]].normal += glm::cross(
			vertices[indices[i + 2]].position - vertices[indices[i]].position,
			vertices[indices[i + 1]].position - vertices[indices[i]].position);

		vertices[indices[i + 1]].normal += glm::cross(
			vertices[indices[i]].position - vertices[indices[i + 1]].position,
			vertices[indices[i + 2]].position - vertices[indices[i + 1]].position);

		vertices[indices[i + 2]].normal += glm::cross(
			vertices[indices[i + 1]].position - vertices[indices[i + 2]].position,
			vertices[indices[i]].position - vertices[indices[i + 2]].position);
	}

	std::for_each(
		vertices.begin(),
		vertices.end(),
		[](VertexFormat& current) { current.normal = glm::normalize(current.normal); });
}