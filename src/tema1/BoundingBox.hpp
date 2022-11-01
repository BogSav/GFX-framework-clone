#pragma once

#include "core/engine.h"
#include "utils/gl_utils.h"

class BoundingBox
{
public:
	BoundingBox(
		bool wireframe = false
	)
		:
		m_wireframe(wireframe)
	{
		m_bottomLeftCorner = { 0,0 };
		m_upperRightCorner = { 0,0 };
	}

	BoundingBox& operator += (const BoundingBox& rhs)
	{
		if (rhs.GetBottomLeftCorner()[0] < this->GetBottomLeftCorner()[0])
			this->m_bottomLeftCorner[0] = rhs.GetBottomLeftCorner()[0];
		if (rhs.GetBottomLeftCorner()[1] < this->GetBottomLeftCorner()[1])
			this->m_bottomLeftCorner[1] = rhs.GetBottomLeftCorner()[1];

		if (rhs.GetUpperRightCorner()[0] > this->m_upperRightCorner[0])
			this->m_upperRightCorner[0] = rhs.GetUpperRightCorner()[0];
		if (rhs.GetUpperRightCorner()[1] > this->m_upperRightCorner[1])
			this->m_upperRightCorner[1] = rhs.GetUpperRightCorner()[1];

		return *this;
	}
	
	void SetBottomLeftCorner(glm::vec2 pos)
	{
		m_bottomLeftCorner = pos;
	}
	void SetUpperRightCorner(glm::vec2 pos)
	{
		m_upperRightCorner = pos;
	}

	glm::vec2 GetBottomLeftCorner() const
	{
		return m_bottomLeftCorner;
	}
	glm::vec2 GetUpperRightCorner() const
	{
		return m_upperRightCorner;
	}

	glm::vec2 GetCenter() const
	{
		return {
			(m_upperRightCorner[0] - m_bottomLeftCorner[0]) / 2.f,
			(m_upperRightCorner[1] - m_bottomLeftCorner[1]) / 2.f
		};
	}

	bool IsInside(glm::vec2 point) const
	{
		return
			!(point[0] > m_upperRightCorner[0] ||
				point[0] < m_bottomLeftCorner[0] ||
				point[1] > m_upperRightCorner[1] ||
				point[1] < m_bottomLeftCorner[1]);
	}

	BoundingBox GetBoundingBoxTransformed(glm::mat3 trMatrix) const
	{
		BoundingBox aux;

		glm::vec3 uprCorner = trMatrix * glm::vec3{ GetUpperRightCorner()[0],GetUpperRightCorner()[1],1 };
		glm::vec3 bolCorner = trMatrix * glm::vec3{ GetBottomLeftCorner()[0],GetBottomLeftCorner()[1],1 };
		aux.SetUpperRightCorner({ uprCorner[0], uprCorner[1] });
		aux.SetBottomLeftCorner({ bolCorner[0], bolCorner[1] });

		return aux;
	}

private:
	const bool m_wireframe;
	glm::vec2 m_bottomLeftCorner;
	glm::vec2 m_upperRightCorner;
};