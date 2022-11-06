#pragma once

#include "Object.hpp"

class Hearts : public Object
{
private:
	class Heart
	{
	public:
		Heart(glm::vec3 v)
		{
			m_geometries.emplace_back(new Circle("stanga", 10, Colors::RED, 1.f, v + glm::vec3{ 1, 2, 0 }));
			m_geometries.emplace_back(new Circle("dreapta", 10, Colors::RED, 1.f, v + glm::vec3{ 2.7, 2, 0 }));
			m_geometries.emplace_back(new Triangle("tri",
				glm::vec2{ m_geometries[0]->GetBottomLeftCorner()[0], 1.62 },
				glm::vec2{ m_geometries[1]->GetUpperRightCorner()[0], 1.62 },
				glm::vec2{ 
					(m_geometries[0]->GetBottomLeftCorner()[0] + m_geometries[1]->GetUpperRightCorner()[0]) / 2,
					0},
				Colors::RED
			));
		}
		void Render(Shader* shader, glm::mat3 modelMatrix, const gfxc::Camera* const camera)
		{
			std::for_each(m_geometries.begin(), m_geometries.end(), [&](const auto& current) {
				current->Render(shader, modelMatrix, camera);
				});
		}
	private:
		std::vector<std::unique_ptr<GeometryObject>> m_geometries;;
	};
public:
	Hearts(TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace viewPortSpace)
		:
		Object(logicSpace, viewPortSpace)
	{
		m_hearts.emplace_back(new Heart({ 0,0,0 }));
		m_hearts.emplace_back(new Heart({ 5,0,0 }));
		m_hearts.emplace_back(new Heart({ 10,0,0 }));
	}
	void Render(Shader* shader, const gfxc::Camera* const camera)
	{
		glm::mat3 modelMatrix = TranformUtils::VisualizationTransf2D(m_logicSpace, m_viewPort);
		modelMatrix *= TranformUtils::Scale(0.5, 0.5);
		std::for_each(m_hearts.begin(), m_hearts.end(), [&](const auto& curr) {
			curr->Render(shader, modelMatrix, camera);
			});
	}
	void LoseAHeart()
	{
		m_hearts.pop_back();
	}
private:
	std::vector<std::unique_ptr<Heart>> m_hearts;
};