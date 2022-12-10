#include "3dGeometryObject.hpp"

#include "tema2/Lighting/LightSource.hpp"
#include <string>
#include <iostream>

GeometryObject3d::GeometryObject3d(
	const Shader* const shader, CustomCamera* const camera, Color color)
	: m_shader(shader), m_camera(camera), m_color(color)
{
}

void GeometryObject3d::Render(const glm::mat4& modelMatrix) const
{
	this->SendDataToShader(modelMatrix);

	m_mesh->Render();
}

void GeometryObject3d::Render(
	const glm::mat4& modelMatrix, const glm::vec3& carPosition, const float& coefficient) const
{
	this->SendDataToShader(modelMatrix);

	int location = glGetUniformLocation(m_shader->program, "CarPosition");
	glUniform3fv(location, 1, glm::value_ptr(carPosition));

	location = glGetUniformLocation(m_shader->program, "CurveCoefficient");
	glUniform1f(location, coefficient);

	m_mesh->Render();
}

void GeometryObject3d::Render(
	const glm::mat4& modelMatrix,
	const glm::vec3& carPosition,
	const glm::vec3& eyePosition,
	const float& coefficient,
	const std::vector<const LightSourceAdapter*>& lightingComponents) const
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
	glUniform1f(location, LightSourceAdapter::materialKd);
	location = glGetUniformLocation(m_shader->program, "material_ks");
	glUniform1f(location, LightSourceAdapter::materialKs);
	location = glGetUniformLocation(m_shader->program, "material_shininess");
	glUniform1i(location, LightSourceAdapter::materialShiness);

	// Send the color of the object
	location = glGetUniformLocation(m_shader->program, "object_color");
	glUniform3fv(location, 1, glm::value_ptr(m_color()));

	// Send the nr of light sources
	location = glGetUniformLocation(m_shader->program, "nrOfLightSources");
	glUniform1i(location, static_cast<int>(lightingComponents.size()));

	// Send the light sources data
	int i = 0;
	std::for_each(
		lightingComponents.begin(),
		lightingComponents.end(),
		[this, &i, &location](const LightSourceAdapter* source)
		{
			glUniform1i(m_shader->ltype[i], source->GetLightType());
			glUniform3fv(m_shader->lposition[i], 1, glm::value_ptr(source->GetPosition()));
			glUniform3fv(m_shader->lcolor[i], 1, glm::value_ptr(source->GetLightColor()()));
			glUniform1f(m_shader->lintensity[i], source->GetLightIntensity());

			if (source->GetLightType() == LightSourceAdapter::SPOT)
			{
				const Spot& spotSource = dynamic_cast<const Spot&>(*source);

				glUniform3fv(m_shader->ldirection[i], 1, glm::value_ptr(spotSource.GetDirection()));
				glUniform1f(m_shader->lcutoff[i], spotSource.GetCutOffAngle());
			}

			i++;
		});

	m_mesh->Render();
}

void GeometryObject3d::Render(const glm::mat4& modelMatrix, const Color& color) const
{
	this->SendDataToShader(modelMatrix);

	int location = glGetUniformLocation(m_shader->program, "UniformColor");
	glUniform3fv(location, 1, glm::value_ptr(color()));

	m_mesh->Render();
}

void GeometryObject3d::Render(
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

void GeometryObject3d::Render(
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

void GeometryObject3d::SendDataToShader(const glm::mat4& modelMatrix) const
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

void GeometryObject3d::CalculateVerticesNormals(
	std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
	for (size_t i = 0; i < indices.size() - 2; i += 3)
	{
		int vertexA = indices[i];
		int vertexB = indices[i + 1];
		int vertexC = indices[i + 2];

		glm::vec3 edgeAB = vertices[vertexB].position - vertices[vertexA].position;
		glm::vec3 edgeAC = vertices[vertexC].position - vertices[vertexA].position;

		glm::vec3 areaWeightedNormal = glm::cross(edgeAB, edgeAC);

		vertices[vertexA].normal += areaWeightedNormal;
		vertices[vertexB].normal += areaWeightedNormal;
		vertices[vertexC].normal += areaWeightedNormal;
	}

	std::for_each(
		vertices.begin(),
		vertices.end(),
		[](VertexFormat& current) { current.normal = glm::normalize(current.normal); });
}