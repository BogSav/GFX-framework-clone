#pragma once

#include "Geometry/Utilities.hpp"
#include "Geometry/Rectangle.hpp"

class Crosshair : public Object
{
public:
	Crosshair() = delete;
	Crosshair(
		TranformUtils::LogicSpace logicSpace,
		TranformUtils::ViewportSpace viewPort,
		float scale,
		Color color = Colors::WHITE)
		:
		Object(logicSpace, viewPort),
		m_color(color),
		m_scale(scale)
	{
		m_components.emplace("vertical-line", new Rectangle("vertical-line",
			{-0.5, -4},
			{-0.5, 4},
			{0.5, 4},
			{0.5, -4},
			m_color,
			0.4f
			));
		m_components.emplace("horizontal-line", new Rectangle("horizontal-line",
			{ -4, -0.5 },
			{ -4, 0.5 },
			{ 4, 0.5 },
			{ 4, -0.5 },
			m_color,
			0.4f
		));
	}

	void UpdatePosition(int screenX, int screenY)
	{
		m_modelMatrix = glm::mat3(1);
		m_modelMatrix *= TranformUtils::Translate((float)screenX, (float)screenY);
		m_modelMatrix *= TranformUtils::Scale(m_scale, m_scale);
	}
private:
	float m_scale;

	Color m_color;
};