#pragma once

#include "Object.hpp"
#include "BoundingBox.hpp"

#include "Triangle.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"

#include "utils/gl_utils.h"

#include "Utilities.hpp"

#include <random>

class Duck
{
public:
	Duck() = delete;
	Duck(TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace viewPort);
	~Duck();

	void Update(float deltaTime);

	bool GotShot(glm::vec2);

	void Render(Shader* shader, const gfxc::Camera* const camera);
	void ForceRenderByCustomModelMatrix(Shader* shader, glm::mat3 modelMatrix, const gfxc::Camera* const camera);

	void CollisionDetectAndAct();

	void SetFlyingSpeed(const float);
	float SetRightArmRotationSpeed() const;
	float SetLeftArmRotationSpeed() const;

	BoundingBox GetBoundingBox() const;
	float GetFlyingSpeed() const;
	float GetRightArmRotationSpeed() const;
	float GetLeftArmRotationSpeed() const;
private:
	CollisionUtils::CollInfo GetCollisionInfo();

	void UpdateAnimation(float deltaTime);
	void UpdatePosition(float deltaTime);

	void UpdateFlyAnimation(float);

	void CalculateBoundingBox();

private:
	std::unordered_map<std::string, std::unique_ptr<Object>> m_components;

	// Random things
	mutable std::mt19937 m_randomEngine;
	std::uniform_real_distribution<float> m_randomStartPositionGenerator;
	std::uniform_real_distribution<float> m_randomStartAngleGenerator;
	std::uniform_int_distribution<int> m_randomChancesForDirectionChange;

	// Logic and vieport spaces related
	TranformUtils::LogicSpace m_logicSpace;
	TranformUtils::ViewportSpace m_viewPort;
	
	BoundingBox m_bbox;
	bool m_isInCollision = false;
	bool m_lastCollisionState = false;
	float m_scale;

	glm::mat3 m_duckModelMatrix = glm::mat3(1);
	glm::mat3 m_VLMatrix = glm::mat3(1);

	glm::mat3 m_rightWingModelMatrix = glm::mat3(1);
	glm::mat3 m_leftWingModelMatrix = glm::mat3(1);

	glm::vec3 m_position = { 0,0,0 };
	glm::vec3 m_flyingDirection = {0, 0, 0 };
	float m_flyingSpeed;

	float m_leftWingRotationAngle = 0;
	float m_leftWingRotationAngularSpeed = 3.14;
	int m_leftWingRotationDirection = 1;

	float m_rightWingRotationAngle = 0;
	float m_rightWingRotationAngularSpeed = 3.14;
	int m_rightWingRotationDirection = 1;

	bool m_wasShot = false;

	int m_dificulty = 1;

	//float m_initialY = 1;
	//float m_flyingAnimationSpeed = 8;
	//float m_flyingAnimationHeight = 2;
	//int m_flyingAnimationDirection = 1;
};