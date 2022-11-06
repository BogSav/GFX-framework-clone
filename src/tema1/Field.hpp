#pragma once

#include "Object.hpp"

class Field : public Object
{
public:
	Field(
		TranformUtils::LogicSpace logicSpace,
		TranformUtils::ViewportSpace viewPortSpace,
		Color color,
		Shader* shader,
		const gfxc::Camera* camera)
		: Object(logicSpace, viewPortSpace, shader, camera), m_color(color)
	{
		m_components.emplace("first", new Circle("1", 30, m_color, 1.f, {1, 0, 0}, 0.4f));
		m_components.emplace("second", new Circle("2", 30, m_color, 1.f, {2, 0, 0}, 0.4f));
		m_components.emplace("third", new Circle("3", 30, m_color, 1.f, {3, 0, 0}, 0.4f));
		m_components.emplace("forth", new Circle("4", 30, m_color, 1.f, {4, 0, 0}, 0.4f));
		m_components.emplace("5", new Circle("5", 30, m_color, 1.f, {5, 0, 0}, 0.4f));
		m_components.emplace("6", new Circle("6", 30, m_color, 1.f, {6, 0, 0}, 0.4f));
	}
	void Render() override
	{
		glm::mat3 VLMatrix = TranformUtils::VisualizationTransf2D(m_logicSpace, m_viewPort);
		m_modelMatrix = VLMatrix;
		Object::Render();
	}

private:
	Color m_color;
};