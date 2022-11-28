#pragma once

#include "tema2/GameComponents/Field.hpp"
#include "tema2/GameComponents/Pista.hpp"
#include "tema2/Geometries/Cuboid.hpp"
#include "tema2/CollisionCore/CollisionEngine.hpp"

#include <random>

class Tree : public GameComponent
{
public:
	static Tree* GenerateRandomTree(
		Shader* shader, const CustomCamera* const camera, const Pista* pista, const Field* field)
	{
		Tree* tmp = nullptr;

		do
		{
			tmp = ReGenerateTree(shader, camera, pista, field);
		} while (tmp == nullptr);

		tmp->Init();

		return tmp;
	}

	glm::vec3 GetTrunkCenter() const
	{
		return m_position + glm::vec3{m_trunkWidth / 2.f, 0, 0}
		+ glm::vec3{0, 0, m_trunkWidth / 2.f};
	}

private:
	static Tree* ReGenerateTree(
		Shader* shader, const CustomCamera* const camera, const Pista* pista, const Field* field)
	{
		Tree* tmp = new Tree(shader, camera);

		std::mt19937 randEngine(std::random_device{}());

		std::uniform_int_distribution<int> posXGenerator(
			field->Getposition().x, field->Getposition().x + field->GetWidth());
		std::uniform_int_distribution<int> posYGenerator(
			field->Getposition().z, field->Getposition().z + field->GetLength());
		std::uniform_real_distribution<float> trunkWidthGenerator(3.f,6.f);
		std::uniform_real_distribution<float> crownWidthGenerator(10.f, 15.f);
		std::uniform_real_distribution<float> trunkHeightGenerator(10.f, 20.f);
		std::uniform_real_distribution<float> crownHeightGenerator(7.f, 14.f);

		tmp->m_position = glm::vec3{posXGenerator(randEngine), 0, posYGenerator(randEngine)};
		tmp->m_trunkHieght = trunkHeightGenerator(randEngine);
		tmp->m_trunkWidth = trunkWidthGenerator(randEngine);
		tmp->m_crownHeight = crownHeightGenerator(randEngine);
		tmp->m_crownWidth = crownWidthGenerator(randEngine);

		if (CollisionEngine::IsOnTrack(pista, tmp))
		{
			delete tmp;
			return nullptr;
		}

		return tmp;
	}

private:
	Tree() = default;
	Tree(Shader* shader, const CustomCamera* const camera) : GameComponent(shader, camera)
	{
		m_position = glm::vec3{0, 0, 0};
		m_trunkHieght = 15;
		m_trunkWidth = 4;
		m_crownHeight = 9;
		m_crownWidth = 15;
	}

	void Init()
	{
		m_geometries.emplace_back(new Cuboid(
			m_shader,
			m_camera,
			m_position,
			m_trunkWidth,
			m_trunkWidth,
			m_trunkHieght,
			Colors::Brown));
		m_geometries.emplace_back(new Cuboid(
			m_shader,
			m_camera,
			m_position
				+ glm::
					vec3{-m_crownWidth / 2.f + m_trunkWidth / 2.f, m_trunkHieght, -m_crownWidth / 2.f + m_trunkWidth / 2.f},
			m_crownWidth,
			m_crownWidth,
			m_crownHeight,
			Colors::TreeGreen));
	}

private:
	glm::vec3 m_position;

	float m_trunkWidth;
	float m_trunkHieght;

	float m_crownWidth;
	float m_crownHeight;
};

struct TreeGenerator
{

};