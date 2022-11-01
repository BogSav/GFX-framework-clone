#pragma once

#include "Object.hpp"

class Triangle : public Object
{
public:
    Triangle(
        const std::string& nume,
        glm::vec2 v1,
        glm::vec2 v2,
        glm::vec2 v3,
        glm::vec3 color,
        const bool wireframe = false)
        :
        Object(nume, wireframe)
    {
        m_vertices.reserve(3);
        m_vertices.emplace_back(glm::vec3{ v1[0], v1[1], 0 }, color);
        m_vertices.emplace_back(glm::vec3{ v2[0], v2[1], 0 }, color);
        m_vertices.emplace_back(glm::vec3{ v3[0], v3[1], 0 }, color);

        m_vertices.reserve(3);
        m_indices.push_back(0);
        m_indices.push_back(1);
        m_indices.push_back(2);

        m_mesh = new Mesh(m_nume);

        if (m_wireframe) {
            m_mesh->SetDrawMode(GL_LINE_LOOP);
        }

        m_mesh->InitFromData(m_vertices, m_indices);

        m_bbox.SetBottomLeftCorner(glm::vec2{ getMinX(), getMinY() });
        m_bbox.SetUpperRightCorner(glm::vec2{ getMaxX(), getMaxY() });
    }

    glm::vec2 GetUpperRightCorner() const override
    {
        return m_bbox.GetUpperRightCorner();
    }
    glm::vec2 GetBottomLeftCorner() const override
    {
        return m_bbox.GetBottomLeftCorner();
    }
};