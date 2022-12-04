#pragma once

#include "LightingComponent.hpp"
#include "tema2/GameComponents/Field.hpp"
#include "tema2/GameComponents/GameComponent.hpp"
#include "tema2/GameComponents/Track.hpp"
#include "tema2/Physics/Collision/CollisionEngine.hpp"

class StreetLight : public GameComponent, public LightingComponent
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
	StreetLight(const Shader* const shader, CustomCamera* const camera)
		: GameComponent(shader, camera)
	{
	}

	static StreetLight* ReGenerateStreetLight(
		const Shader* const shader,
		CustomCamera* const camera,
		const Track* pista,
		const Field* field)
	{
		StreetLight* tmp = new StreetLight(shader, camera);

		std::mt19937 randEngine(std::random_device{}());

		std::uniform_real_distribution<float> posXGenerator(
			field->Getposition().x, field->Getposition().x + field->GetWidth());
		std::uniform_real_distribution<float> posYGenerator(
			field->Getposition().z, field->Getposition().z + field->GetLength());
		std::uniform_real_distribution<float> baseWidthGenerator(3.f, 6.f);
		std::uniform_real_distribution<float> baseHeightGenerator(10.f, 20.f);
		std::uniform_real_distribution<float> headHeightGenerator(7.f, 14.f);

		tmp->m_position = glm::vec3{posXGenerator(randEngine), 0, posYGenerator(randEngine)};
		tmp->m_baseHeight = baseHeightGenerator(randEngine);
		tmp->m_baseWidth = baseWidthGenerator(randEngine);
		tmp->m_headHeight = headHeightGenerator(randEngine);
		tmp->m_headWidth = tmp->m_baseWidth;

		if (CollisionEngine::IsOnTrack(pista, tmp))
		{
			delete tmp;
			return nullptr;
		}

		return tmp;
	}

	void Init()
	{
		m_geometries.emplace_back(new Polyhedron3d(
			m_shader, m_camera, m_position, m_baseWidth, m_baseWidth, m_baseHeight, Colors::Gray));
		m_geometries.emplace_back(new Polyhedron3d(
			m_shader,
			m_camera,
			m_position
				+ glm::
					vec3{-m_headWidth / 2.f + m_baseWidth / 2.f, m_baseHeight, -m_headWidth / 2.f + m_baseWidth / 2.f},
			m_headWidth,
			m_headWidth,
			m_headHeight,
			Colors::White));
	}

	const glm::vec3& GetBasePosition() const
	{
		return m_position + glm::vec3{m_baseWidth / 2.f, 0, 0} + glm::vec3{0, 0, m_baseWidth / 2.f};
	}

	const glm::vec3& operator()() const override { return this->GetLightSourcePosition(); }
	const glm::vec3& GetLightSourcePosition() const override
	{
		return m_position + glm::vec3{0, m_baseHeight + m_headHeight / 2.f, 0};
	}

private:
	float m_baseWidth = 0.f;
	float m_baseHeight = 0.f;

	float m_headWidth = 0.f;
	float m_headHeight = 0.f;
};