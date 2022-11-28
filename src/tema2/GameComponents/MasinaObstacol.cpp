#include "MasinaObstacol.hpp"

#include "tema2/Geometries/Cuboid.hpp"
#include "tema2/Utilities/Transformations.hpp"

#include <iostream>
#include <numbers>
#include <random>

MasinaObstacol* MasinaObstacol::CreateNewNPC(
	const std::vector<glm::vec3>& intPts, Shader* shader, const CustomCamera* camera)
{
	MasinaObstacol* tmp = new MasinaObstacol(intPts, shader, camera);

	tmp->m_speed = 20;
	tmp->m_width = 2.f;
	tmp->m_length = 3.f;
	tmp->m_height = 5;

	tmp->m_distanceFromInteriorPoint = 2;

	tmp->m_currentQuad = 0;

	tmp->m_color = Colors::Cyan;

	tmp->Init();

	return tmp;
}

MasinaObstacol* MasinaObstacol::CreateNewNPCRandomized(
	const std::vector<glm::vec3>& intPts, Shader* shader, const CustomCamera* camera)
{
	MasinaObstacol* tmp = new MasinaObstacol(intPts, shader, camera);

	std::mt19937 randEngine(std::random_device{}());
	std::uniform_int_distribution<int> quadGenerator(0, intPts.size() - 1);
	std::uniform_real_distribution<float> widthGenerator(2.5f, 5.f);
	std::uniform_real_distribution<float> distanceGenerator(7.f, 16.f);
	std::uniform_real_distribution<float> speedGenerator(15.f, 15.f);

	tmp->m_speed = speedGenerator(randEngine);
	tmp->m_width = widthGenerator(randEngine);
	tmp->m_length = tmp->m_width * 2.f;
	tmp->m_height = 5;

	tmp->m_distanceFromInteriorPoint = distanceGenerator(randEngine);

	tmp->m_currentQuad = quadGenerator(randEngine);

	tmp->m_color = Colors::Cyan;

	tmp->Init();

	return tmp;
}

MasinaObstacol::MasinaObstacol(
	const std::vector<glm::vec3>& intPts, Shader* shader, const CustomCamera* camera)
	: m_interiorPoints(intPts), GameComponent(shader, camera)
{
}

void MasinaObstacol::Update(float deltaTime)
{
	UpdatePosition(deltaTime);

	if (glm::dot(GetCenterPoint() - m_interiorPoints[m_currentQuad + 1], GetAntiMoveDirection())
		< 0)
	{
		m_currentQuad++;

		if (m_currentQuad == m_interiorPoints.size() - 1)
			m_currentQuad = 0;

		UpdateOrientation();
		UpdateDirection();
	}

	UpdateModelMatrix();
}

void MasinaObstacol::UpdatePosition(float deltaTime)
{
	m_position += (m_direction * m_speed * deltaTime);
	float currentDistance = GetDistanceToCurrentFragment();
	if (currentDistance < m_distanceFromInteriorPoint)
	{
		m_position += GetNormalToDirection() * (m_distanceFromInteriorPoint - currentDistance);
	}
}

void MasinaObstacol::UpdateDirection()
{
	m_direction = -1.f * GetAntiMoveDirection();
}

void MasinaObstacol::UpdateOrientation()
{
	float dotPr = glm::dot(m_direction, glm::vec3{1, 0, 0});
	float crossPr = glm::cross(m_direction, glm::vec3{-1, 0, 0}).y;

	float currentAngle = glm::acos(glm::abs(dotPr));

	if (crossPr < 0 && dotPr > 0)
	{
		currentAngle += 3.f * 3.14 / 2.f;
	}
	else if (crossPr < 0 && dotPr < 0)
	{
		currentAngle += 3.14;
	}
	else if (crossPr > 0 && dotPr < 0)
	{
		currentAngle += 3.14 / 2;
	}

	m_orientationAngle = currentAngle - 3.14 / 2;
}

void MasinaObstacol::UpdateModelMatrix()
{
	m_modelMatrix = glm::mat4(1);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	// m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3{m_width / 2.f, 0, m_length / 2.f});
	m_modelMatrix = glm::rotate(m_modelMatrix, m_orientationAngle, glm::vec3{0, 1, 0});
	// m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3{-m_width / 2.f, 0, -m_length / 2.f});
}

void MasinaObstacol::ComputeInitialPosition()
{
	m_position = utils::GetInterpolatedPoint(
					 m_interiorPoints[m_currentQuad], m_interiorPoints[m_currentQuad + 1], 0.5f)
		+ GetNormalToDirection() * m_distanceFromInteriorPoint;
}

glm::vec3 MasinaObstacol::GetAntiMoveDirection() const
{
	return glm::normalize(m_interiorPoints[m_currentQuad] - m_interiorPoints[m_currentQuad + 1]);
}

glm::vec3 MasinaObstacol::GetNormalToDirection() const
{
	return glm::normalize(glm::cross(glm::vec3{0, 1, 0}, GetAntiMoveDirection()));
}

glm::vec3 MasinaObstacol::GetCenterPoint() const
{
	return m_position /*+ GetAntiMoveDirection() * m_length / 2.f
		+ GetNormalToDirection() * m_width / 2.f*/
		;
}

float MasinaObstacol::GetDistanceToCurrentFragment() const
{
	return glm::abs(
			   (m_interiorPoints[m_currentQuad + 1].x - m_interiorPoints[m_currentQuad].x)
				   * (m_interiorPoints[m_currentQuad].z - m_position.z)
			   - (m_interiorPoints[m_currentQuad].x - m_position.x)
				   * (m_interiorPoints[m_currentQuad + 1].z - m_interiorPoints[m_currentQuad].z))
		/ glm::sqrt(
			   glm::pow(
				   m_interiorPoints[m_currentQuad + 1].x - m_interiorPoints[m_currentQuad].x, 2.f)
			   + glm::pow(
				   m_interiorPoints[m_currentQuad + 1].z - m_interiorPoints[m_currentQuad].z, 2.f));
}

void MasinaObstacol::Init()
{
	UpdateDirection();
	ComputeInitialPosition();
	UpdateOrientation();

	m_geometries.emplace_back(
		new Cuboid(m_shader, m_camera, glm::vec3{0, 0, 0}, m_width, m_length, m_height, m_color));
}
