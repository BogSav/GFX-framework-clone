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
	Object(TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace m_viewPort, Shader* shader, const gfxc::Camera* camera)
		:
		m_shader(shader),
		m_camera(camera),
		m_logicSpace(logicSpace),
		m_viewPort(m_viewPort)
	{}

	virtual void Render()
	{
		std::for_each(m_components.begin(), m_components.end(), [&](const auto& current) {
			current.second->Render(m_shader, m_modelMatrix, m_camera);
			});
	}
protected:
	std::unordered_map<std::string, std::unique_ptr<GeometryObject>> m_components;

	TranformUtils::LogicSpace m_logicSpace;
	TranformUtils::ViewportSpace m_viewPort;

	Shader* m_shader;
	const gfxc::Camera* m_camera;

	glm::mat3 m_modelMatrix = glm::mat3(1);
};