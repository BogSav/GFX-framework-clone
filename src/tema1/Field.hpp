#pragma once

#include "Object.hpp"

class Field : public Object
{
public:
	Field(TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace viewPortSpace, Color color)
		:
		Object(logicSpace, viewPortSpace),
		m_color(color)
	{
		m_components.emplace("first", new Circle("first",
			30,
			m_color,
			1.f,
			{1,0,0},
			0.4));
		m_components.emplace("second", new Circle("second",
			30,
			m_color,
			1.f,
			{ 2,0,0 },
			0.4));
		m_components.emplace("third", new Circle("third",
			30,
			m_color,
			1.f,
			{ 3,0,0 },
			0.4));
		m_components.emplace("forth", new Circle("forth",
			30,
			m_color,
			1.f,
			{ 4,0,0 },
			0.4));
		m_components.emplace("5", new Circle("5",
			30,
			m_color,
			1.f,
			{ 5,0,0 },
			0.4));
		m_components.emplace("6", new Circle("6",
			30,
			m_color,
			1.f,
			{ 6,0,0 },
			0.4));
	}
	void Render(Shader* shader, const gfxc::Camera* const camera) override
	{
		glm::mat3 VLMatrix = TranformUtils::VisualizationTransf2D(m_logicSpace, m_viewPort);
		m_modelMatrix = VLMatrix;
		Object::Render(shader, camera);
	}

private:
	Color m_color;
};