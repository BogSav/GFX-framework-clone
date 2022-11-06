#pragma once

#include "Hearts.hpp"
#include "Object.hpp"

#include <random>

class Duck : public Object
{
public:
	struct DuckProperties
	{
	public:
		float scale = 5.f;

		float flyingSpeed = 10.f;

		float leftWingRotationAngularSpeed = std::numbers::pi_v<float>;

		float rightWingRotationAngularSpeed = std::numbers::pi_v<float>;

		float deadFallingSpeed = 20.f;
		float deadDramaticRotationSpeed = std::numbers::pi_v<float> / 2.f;

		float slaveryTime = 10.f;

		float timeBetweenRandomPositionChanges = 1.5f;

		int duckDificulty = 1;

	public:
		static DuckProperties* GenerateDuckPropertiesAccordingToLevel(int currentLevel)
		{
			DuckProperties* props = new DuckProperties();

			props->duckDificulty = (int)std::ceil(currentLevel / 2.f);

			props->flyingSpeed = 5.f * (props->duckDificulty + 1.f);

			props->slaveryTime = 10.f - props->duckDificulty;

			return props;
		}
	};

public:
	Duck() = delete;
	Duck(
		TranformUtils::LogicSpace,
		TranformUtils::ViewportSpace,
		Shader*,
		const gfxc::Camera*,
		DuckProperties*);

	void Update(float deltaTime);

	bool GotShot(glm::vec2);
	bool IsDead() const { return m_isDead; };
	bool IsFree() const { return m_IsFree; };
	void SetFree() { m_IsFree = true; };

	void Render() override;
	void ForceRenderByCustomModelMatrix(glm::mat3 modelMatrix);

	void CollisionDetectAndAct();

	BoundingBox GetBoundingBox() const;

	glm::mat3 GetModelMatrix() const { return m_modelMatrix; };
	BoundingBox GetRawBBox() const { return m_bbox; };
	void SetTimeServed() { m_timeBeingASlave = m_props->slaveryTime; };
	float GetTimeBeingASlave() { return m_timeBeingASlave; };
	float GetSlaveryTime() { return m_props->slaveryTime; };

private:
	CollisionUtils::CollInfo GetCollisionInfo();

	void GenerateBodyComponents();

	void UpdateAnimation(float deltaTime);
	void UpdatePosition(float deltaTime);

	void UpdateFlyAnimation(float);

	void CalculateBoundingBox();

	void OrientModel();

private:
	std::unordered_map<std::string, std::unique_ptr<GeometryObject>> m_components;

	// Random things
	mutable std::mt19937 m_randomEngine;
	std::uniform_real_distribution<float> m_randomStartPositionGenerator;
	std::uniform_real_distribution<float> m_randomStartAngleGenerator;
	std::uniform_real_distribution<float> m_randomPositionRotationAngleGenerator;
	std::uniform_real_distribution<float> m_randomReflexionChancesGenerator;

	BoundingBox m_bbox;

	glm::mat3 m_VLMatrix = glm::mat3(1);
	glm::mat3 m_rightWingModelMatrix = glm::mat3(1);
	glm::mat3 m_leftWingModelMatrix = glm::mat3(1);

	glm::vec3 m_position = {0, 0, 0};
	glm::vec3 m_flyingDirection = {0, 0, 0};

	float m_leftWingRotationAngle = 0.f;
	int m_leftWingRotationDirection = 1;

	float m_rightWingRotationAngle = 0.f;
	int m_rightWingRotationDirection = 1;

	float m_deadDramaticRotationAngle = 0.f;

	float m_timeBeingASlave = 0;

	int m_modelOrientation = 1;
	int m_nuVreauSaFaAsta = 1;

	int m_nrOfShots = 0;
	bool m_isDead = false;
	bool m_IsFree = false;

	int m_currentPositionChange = 1;

	std::unique_ptr<DuckProperties> m_props;
};