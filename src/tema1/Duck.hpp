#pragma once

#include "Object.hpp"

#include <random>

class Duck : public Object
{
public:
	Duck() = delete;
	Duck(TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace viewPort);

	void Update(float deltaTime);

	bool GotShot(glm::vec2);
	bool IsDead() const { return m_isDead; };
	bool IsFree() const { return m_IsFree; };
	void SetFree() { m_IsFree = true; };

	void Render(Shader* shader, const gfxc::Camera* const camera) override;
	void ForceRenderByCustomModelMatrix(Shader* shader, glm::mat3 modelMatrix, const gfxc::Camera* const camera);

	void CollisionDetectAndAct();

	BoundingBox GetBoundingBox() const;

	glm::mat3 GetModelMatrix() const { return m_modelMatrix; };
	BoundingBox GetRawBBox() const { return m_bbox; };
private:
	CollisionUtils::CollInfo GetCollisionInfo();

	void UpdateAnimation(float deltaTime);
	void UpdatePosition(float deltaTime);

	void UpdateFlyAnimation(float);

	void CalculateBoundingBox();

private:
	std::unordered_map<std::string, std::unique_ptr<GeometryObject>> m_components;

	// Random things
	mutable std::mt19937 m_randomEngine;
	std::uniform_real_distribution<float> m_randomStartPositionGenerator;
	std::uniform_real_distribution<float> m_randomStartAngleGenerator;
	std::uniform_int_distribution<int> m_randomChancesForDirectionChange;
	
	BoundingBox m_bbox;
	float m_scale;

	glm::mat3 m_VLMatrix = glm::mat3(1);

	glm::mat3 m_rightWingModelMatrix = glm::mat3(1);
	glm::mat3 m_leftWingModelMatrix = glm::mat3(1);

	glm::vec3 m_position = { 0,0,0 };
	glm::vec3 m_flyingDirection = {0, 0, 0 };
	float m_flyingSpeed;

	float m_leftWingRotationAngle = 0.f;
	float m_leftWingRotationAngularSpeed = 3.14f;
	int m_leftWingRotationDirection = 1;

	float m_rightWingRotationAngle = 0.f;
	float m_rightWingRotationAngularSpeed = 3.14f;
	int m_rightWingRotationDirection = 1;

	float m_deadFallingSpeed = 20;
	float m_deadDramaticRotationSpeed = 3.14f / 2.f;
	float m_deadDramaticRotationAngle = 0.f;

	int m_modelOrientation = 1;
	int m_nuVreauSaFaAsta = 1;

	int m_nrOfShots = 0;
	bool m_isDead = false;
	int m_duckDificulty = 1;
	bool m_IsFree = false;
};