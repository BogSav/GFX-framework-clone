#pragma once

#include "GeometryObject.hpp"

class Rectangle : public GeometryObject
{
public:
	Rectangle(
		const std::string& nume,
		glm::vec2 v1,
		glm::vec2 v2,
		glm::vec2 v3,
		glm::vec2 v4,
		Color color,
		float zIndex = 0.f,
		bool wireFrame = false)
		: GeometryObject(nume, wireFrame, zIndex)
	{
		m_vertices.reserve(4);
		m_vertices.emplace_back(glm::vec3{v1[0], v1[1], 0}, color);
		m_vertices.emplace_back(glm::vec3{v2[0], v2[1], 0}, color);
		m_vertices.emplace_back(glm::vec3{v3[0], v3[1], 0}, color);
		m_vertices.emplace_back(glm::vec3{v4[0], v4[1], 0}, color);

		m_vertices.reserve(3);
		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
		m_indices.push_back(3);

		m_mesh = std::make_unique<Mesh>(m_nume);

		if (m_wireframe)
		{
			m_mesh->SetDrawMode(GL_LINE_LOOP);
		}
		else
		{
			m_indices.push_back(0);
			m_indices.push_back(2);
		}

		m_mesh->InitFromData(m_vertices, m_indices);

		m_bbox.SetBottomLeftCorner(glm::vec2{getMinX(), getMinY()});
		m_bbox.SetUpperRightCorner(glm::vec2{getMaxX(), getMaxY()});
	}
	Rectangle(
		const std::string nume,
		glm::vec2 bottomLeft,
		float width,
		float height,
		Color color,
		float zIndex = 0.f,
		bool wireFrame = false)
		: Rectangle(
			nume,
			bottomLeft,
			{bottomLeft[0], bottomLeft[1] + height},
			{bottomLeft[0] + width, bottomLeft[1] + height},
			{bottomLeft[0] + width, bottomLeft[1]},
			color,
			zIndex,
			wireFrame)
	{
	}
	Rectangle(const std::string& nume, BoundingBox bbox, Color color, bool wireFrame = true)
		: Rectangle(
			nume,
			bbox.GetBottomLeftCorner(),
			{bbox.GetBottomLeftCorner()[0], bbox.GetUpperRightCorner()[1]},
			bbox.GetUpperRightCorner(),
			{bbox.GetUpperRightCorner()[0], bbox.GetBottomLeftCorner()[1]},
			color,
			wireFrame)
	{
	}
};