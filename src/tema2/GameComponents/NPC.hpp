#pragma once

#include "GameComponent.hpp"
#include <numbers>

class NPC : public GameComponent
{
public:
	static NPC* CreateNewNPCRandomized(
		const std::vector<glm::vec3>& intPts, const Shader* const shader, CustomCamera* const camera);

	friend class CollisionEngine;

public:
	void Update(float deltaTime) override;

	int& GetTotalNrOfQuads();

private:
	NPC() = delete;
	NPC(const std::vector<glm::vec3>&, const Shader* const, CustomCamera* const);

	void Init();

	void UpdatePosition(float);
	void UpdateDirection();
	void UpdateOrientation();
	void UpdateModelMatrix();

	void ComputeInitialPosition();

	glm::vec3 GetAntiMoveDirection() const;
	glm::vec3 GetNormalToDirection() const;
	glm::vec3 GetCenterPoint() const;
	float GetDistanceToCurrentFragment() const;

private:
	const std::vector<glm::vec3>& m_interiorPoints;

	glm::vec3 m_direction;
	glm::vec3 m_position;
	float m_orientationAngle;

	Color m_color;

	float m_speed;
	int m_currentQuad;
	int m_totalNrOfQuads;

	float m_width;
	float m_length;
	float m_height;

	float m_distanceFromInteriorPoint;

	const float piOver2 = std::numbers::pi_v<float> / 2.0f;
};
