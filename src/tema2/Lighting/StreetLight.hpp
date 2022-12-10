#pragma once

#include "LightSource.hpp"
#include "tema2/GameComponents/Field.hpp"
#include "tema2/GameComponents/GameComponent.hpp"
#include "tema2/GameComponents/Track.hpp"
#include "tema2/Physics/Collision/CollisionEngine.hpp"

class StreetLight : public GameComponent, public DirectionalLight
{
public:
	static StreetLight* GenerateRandomStreetLight(
		const Shader* const shader,
		CustomCamera* const camera,
		const Track* pista,
		const Field* field)
	{
		StreetLight* tmp = nullptr;

		do
		{
			tmp = ReGenerateStreetLight(shader, camera, pista, field);
		} while (tmp == nullptr);

		tmp->Init();

		return tmp;
	}

	friend class CollisionEngine;
	friend class GeometryObject;

private:
	StreetLight() = delete;
	StreetLight(
		const Shader* const shader,
		CustomCamera* const camera,
		const glm::vec3& basePosition,
		const glm::vec3& lightPosition,
		const Color& color,
		const float& intensity)
		: GameComponent(shader, camera),
		  DirectionalLight(lightPosition, color, intensity),
		  m_basePosition(basePosition)
	{
	}

	static StreetLight* ReGenerateStreetLight(
		const Shader* const shader,
		CustomCamera* const camera,
		const Track* pista,
		const Field* field)
	{
		std::mt19937 randEngine(std::random_device{}());

		std::uniform_real_distribution<float> posXGenerator(
			field->Getposition().x, field->Getposition().x + field->GetWidth());
		std::uniform_real_distribution<float> posZGenerator(
			field->Getposition().z, field->Getposition().z + field->GetLength());
		std::uniform_real_distribution<float> baseWidthGenerator(3.f, 6.f);
		std::uniform_real_distribution<float> baseHeightGenerator(10.f, 20.f);
		std::uniform_real_distribution<float> headHeightGenerator(7.f, 14.f);

		const glm::vec3 position =
			glm::vec3{posXGenerator(randEngine), 0, posZGenerator(randEngine)};
		const float baseHeight = baseHeightGenerator(randEngine);
		const float baseWidth = baseWidthGenerator(randEngine);
		const float headHeight = headHeightGenerator(randEngine);
		const float headWidth = baseWidth;

		StreetLight* tmp = new StreetLight(
			shader,
			camera,
			position,
			position + glm::vec3{0, baseHeight + headHeight, 0}
				+ glm::vec3{baseHeight / 2.f, 0, baseWidth / 2.f},
			Colors::White,
			1.0f);

		if (CollisionEngine::IsOnTrack(pista, tmp))
		{
			delete tmp;
			return nullptr;
		}

		tmp->m_baseWidth = baseWidth;
		tmp->m_baseHeight = baseHeight;
		tmp->m_headHeight = headHeight;
		tmp->m_headWidth = headWidth;

		return tmp;
	}

	void Init()
	{
		m_geometries.emplace_back(new Polyhedron3d(
			m_shader,
			m_camera,
			m_basePosition,
			m_baseWidth,
			m_baseWidth,
			m_baseHeight,
			Colors::Gray));
		m_geometries.emplace_back(new Polyhedron3d(
			m_shader,
			m_camera,
			m_basePosition
				+ glm::
					vec3{-m_headWidth / 2.f + m_baseWidth / 2.f, m_baseHeight, -m_headWidth / 2.f + m_baseWidth / 2.f},
			m_headWidth,
			m_headWidth,
			m_headHeight,
			Colors::White));
	}

	glm::vec3 GetBasePosition() const
	{
		return m_basePosition + glm::vec3{m_baseWidth / 2.f, 0, 0}
		+ glm::vec3{0, 0, m_baseWidth / 2.f};
	}

private:
	glm::vec3 m_basePosition;

	float m_baseWidth = 0.f;
	float m_baseHeight = 0.f;

	float m_headWidth = 0.f;
	float m_headHeight = 0.f;
};