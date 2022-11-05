#pragma once

#include "Geometry/Utilities.hpp"

#include "Geometry/GeometryObject.hpp"
#include "Geometry/Utilities.hpp"

#include "Geometry/Triangle.hpp"
#include "Geometry/Circle.hpp"
#include "Geometry/Rectangle.hpp"

#include "Colors.hpp"

class Object
{
public:
	Object() = delete;
	Object(TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace m_viewPort)
		:
		m_logicSpace(logicSpace),
		m_viewPort(m_viewPort)
	{}

	virtual void Render(Shader* shader, const gfxc::Camera* const camera)
	{
		std::for_each(m_components.begin(), m_components.end(), [&](const auto& current) {
			current.second->Render(shader, m_modelMatrix, camera);
			});
	}
protected:
	std::unordered_map<std::string, std::unique_ptr<GeometryObject>> m_components;

	TranformUtils::LogicSpace m_logicSpace;
	TranformUtils::ViewportSpace m_viewPort;

	glm::mat3 m_modelMatrix = glm::mat3(1);
};