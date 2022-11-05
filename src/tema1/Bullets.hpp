#pragma once

#include "Object.hpp"

class Bullets : public Object
{
private:
	class Bullet
	{
	public:
		Bullet(glm::vec3 v)
		{
			m_geometries.emplace_back(new Rectangle("body",
				{v[0], 0},
				0.8f,
				1.f,
				Colors::LIGHT_GOLD, 
				0.7f));
			m_geometries.emplace_back(new Circle("head",
				10,
				Colors::DARK_GOLD,
				0.4f,
				glm::vec3{ 0.4f + v[0], 1.f, 0},
				0.6f));
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
	Bullets(TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace viewPortSpace)
		:
		Object(logicSpace, viewPortSpace),
		m_nrOfBullets(3)
	{
		ResetBullets();
	}
	void Render(Shader* shader, const gfxc::Camera* const camera)
	{
		glm::mat3 modelMatrix = TranformUtils::VisualizationTransf2D(m_logicSpace, m_viewPort);
		std::for_each(m_bullets.begin(), m_bullets.end(), [&](const auto& curr) {
			curr->Render(shader, modelMatrix, camera);
			});
	}
	void ShotBullet()
	{
		m_bullets.pop_back();
	}
	bool IsCartridgeEmpty()
	{
		return m_bullets.size() == 0;
	}
	void ResetBullets()
	{
		float position = 0;
		for (size_t i = 0; i < m_nrOfBullets; i++, position += 1.2f)
		{
			m_bullets.emplace_back(new Bullet({ position, 0, 0 }));
		}
	}
private:
	std::vector<std::unique_ptr<Bullet>> m_bullets;
	int m_nrOfBullets;
};