#pragma once

#include "core/engine.h"
#include "components/simple_scene.h"

#include "BoundingBox.hpp"
#include "Colors.hpp"

class GeometryObject
{
public:
	GeometryObject() = delete;
	GeometryObject(const std::string& nume, bool wireframe, float zIndex = 0.f)
		:
		m_nume(nume),
		m_wireframe(wireframe),
		m_zIndex(zIndex)
	{}
	
	BoundingBox GetBoundingBox() const
	{
		return m_bbox;
	}

	glm::vec2 GetUpperRightCorner() const
	{
		return m_bbox.GetUpperRightCorner();
	}
	glm::vec2 GetBottomLeftCorner() const
	{
		return m_bbox.GetBottomLeftCorner();
	}

	void Render(Shader* shader, const glm::mat3& modelMatrix, const gfxc::Camera* const camera) const
	{
		if (!m_mesh || !shader || !shader->program)
			return;

		shader->Use();
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

		glm::mat3 mm = modelMatrix;
		glm::mat4 model = glm::mat4(
			mm[0][0], mm[0][1], mm[0][2], 0.f,
			mm[1][0], mm[1][1], mm[1][2], 0.f,
			0.f, 0.f, mm[2][2], 0.f,
			mm[2][0], mm[2][1], 0.f, 1.f);

		model = model * glm::transpose(
			glm::mat4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, m_zIndex,
				0, 0, 0, 1
			)
		);

		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
		m_mesh->Render();
	}

protected:
	float getMaxY() const
	{
		return std::max_element(m_vertices.begin(), m_vertices.end(), [](const auto& a, const auto& b) {
			return a.position[1] < b.position[1];
			})->position[1];
	}
	float getMaxX() const
	{
		return std::max_element(m_vertices.begin(), m_vertices.end(), [](const auto& a, const auto& b) {
			return a.position[0] < b.position[0];
			})->position[0];
	}
	float getMinY() const
	{
		return std::min_element(m_vertices.begin(), m_vertices.end(), [](const auto& a, const auto& b) {
			return a.position[1] < b.position[1];
			})->position[1];
	}
	float getMinX() const
	{
		return std::min_element(m_vertices.begin(), m_vertices.end(), [](const auto& a, const auto& b) {
			return a.position[0] < b.position[0];
			})->position[0];
	}

protected:
	BoundingBox m_bbox;
	const std::string m_nume;
	std::unique_ptr<Mesh> m_mesh = nullptr;
	std::vector<VertexFormat> m_vertices;
	std::vector<unsigned int> m_indices;
	bool m_wireframe;
	float m_zIndex;
};

