#pragma once

#include "GameComponent.hpp"

#include "Field.hpp"

#include <random>

void GameComponent::GenerateRandomPositionOnField(const Field* field)
{
	glm::vec3 bottomLeftPoint = glm::vec3{
		field->GetBBox().GetMinCorner().x,
		field->GetBBox().GetMinCorner().y,
		field->GetBBox().GetMaxCorner().z};

	glm::vec3 bottomRightPoint = glm::vec3{
		field->GetBBox().GetMaxCorner().x,
		field->GetBBox().GetMinCorner().y,
		field->GetBBox().GetMaxCorner().z};

	glm::vec3 topLeftPoint = glm::vec3{
		field->GetBBox().GetMinCorner().x,
		field->GetBBox().GetMaxCorner().y,
		field->GetBBox().GetMinCorner().z};

	std::mt19937 rand(std::random_device{}());
	std::uniform_real_distribution<float> randomXInterpolationValue(0, 1);
	std::uniform_real_distribution<float> randomYInterpolationValue(0, 0.4f);

	glm::vec3 position1 =
		utils::GetInterpolatedPoint(bottomLeftPoint, bottomRightPoint, randomXInterpolationValue(rand));
	glm::vec3 position2 =
		utils::GetInterpolatedPoint(bottomLeftPoint, topLeftPoint, randomYInterpolationValue(rand));

	m_position = glm::vec3{position1.x, position2.y, position2.z};
}

void GameComponent::Activate(const Field* field)
{
	m_isActive = true;
	GenerateRandomPositionOnField(field);
}

void GameComponent::Deactivate()
{
	m_isActive = false;
}
