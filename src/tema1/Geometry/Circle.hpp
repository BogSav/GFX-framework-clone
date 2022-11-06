#pragma once

#include "GeometryObject.hpp"

#include <numbers>

class Circle : public GeometryObject
{
public:
	Circle(
		const std::string& nume,
		int nrOfSteps,
		glm::vec3 color,
		float scale = 1.f,
		glm::vec3 origin = glm::vec3{0, 0, 0},
		const float zIndex = 0,
		const bool wireframe = false)
		: GeometryObject(nume, wireframe, zIndex),
		  m_origin(origin),
		  m_scale(scale),
		  m_nrOfSteps(nrOfSteps)
	{
		m_vertices.emplace_back(m_origin + glm::vec3{0, 0, 0}, color);
		m_vertices.emplace_back(m_origin + glm::vec3{1, 0, 0} * m_scale, color);

		float angularStep = std::numbers::pi_v<float> / m_nrOfSteps;
		int nrOfTriangles = m_nrOfSteps * 4 - 1;
		int trNr = 0;
		for (float currentAngle = 0; trNr < nrOfTriangles; currentAngle += angularStep, trNr++)
		{
			m_vertices.emplace_back(
				m_origin + glm::vec3{sinf(currentAngle), cosf(currentAngle), 0} * m_scale, color);

			m_indices.push_back(trNr + 1);
			m_indices.push_back(0);
			m_indices.push_back(trNr + 2);
		}

		m_indices.push_back(1);
		m_indices.push_back(0);
		m_indices.push_back(nrOfTriangles + 1);

		m_mesh = std::make_unique<Mesh>(m_nume);

		if (m_wireframe)
		{
			m_mesh->SetDrawMode(GL_LINE_LOOP);
		}

		m_mesh->InitFromData(m_vertices, m_indices);

		m_bbox.SetBottomLeftCorner(glm::vec2{getMinX(), getMinY()});
		m_bbox.SetUpperRightCorner(glm::vec2{getMaxX(), getMaxY()});
	}

private:
	int m_nrOfSteps;
	glm::vec3 m_origin;
	float m_scale;
};