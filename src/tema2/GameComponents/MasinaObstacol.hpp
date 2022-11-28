#pragma once

#include "GameComponent.hpp"

class MasinaObstacol : public GameComponent
{
public:
	static MasinaObstacol* CreateNewNPC(
		const std::vector<glm::vec3>& intPts, Shader* shader, const CustomCamera* camera);
	static MasinaObstacol* CreateNewNPCRandomized(
		const std::vector<glm::vec3>& intPts, Shader* shader, const CustomCamera* camera);

	friend class CollisionEngine;

public:
	void Update(float deltaTime) override;

private:
	MasinaObstacol() = delete;
	MasinaObstacol(const std::vector<glm::vec3>&, Shader* shader, const CustomCamera* camera);

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

	float m_width;
	float m_length;
	float m_height;

	float m_distanceFromInteriorPoint;
};
