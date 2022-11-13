#pragma once

#include "Hearts.hpp"
#include "Object.hpp"
#include "TimeCounter.hpp"
#include "ShotAnimation.hpp"

#include <random>

class Duck : public Object
{
public:
	enum Difficulties
	{
		BABY = 1,
		EASY,
		MEDIUM,
		HARD,
		IMPOSSIBLE
	};

private:
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

		int requieredShots = 1;

		int duckDificulty = Difficulties::BABY;

		float shotAnimationDuration = 0.45f;

		float resetTime = 2.f;
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

	void Shot(glm::vec2);
	void Shot(BoundingBox);

	bool IsDead() const { return m_isDead; };
	bool IsFree() const { return m_IsFree; };
	bool IsInGame() const { return !(m_IsFree || m_isDead); };

	void SetFree();
	void SetTimeServed() { m_aliveTimer.ResetTime(); };

	void SetActive()
	{
		m_idle = false;
		m_aliveTimer.ResetTime();
	};

	void Render() override;
	void ForceRenderByCustomModelMatrix(glm::mat3 modelMatrix);

	BoundingBox GetBoundingBox() const;

	glm::mat3 GetModelMatrix() const { return m_modelMatrix; };
	BoundingBox GetRawBBox() const { return m_bbox; };

	float GetAliveTime() { return m_aliveTimer.GetTimeElaspsed(); };
	float GetSlaveryTime() { return m_props->slaveryTime; };

	int GetRemainingRequiredShots() { return m_props->requieredShots - m_nrOfShots; };

	bool SentenceIsOver() { return m_aliveTimer.TimePassedValue(m_props->slaveryTime); };

	bool IsFreeForReset();

public:
	static DuckProperties* GenerateDuckPropertiesAccordingToLevel(int currentLevel)
	{
		DuckProperties* props = new DuckProperties();

		props->duckDificulty = (int)std::ceil(currentLevel / 2.f);

		props->flyingSpeed = 5.f * (props->duckDificulty + 1.f);

		props->slaveryTime = 10.f - props->duckDificulty;

		props->requieredShots = props->duckDificulty <= 3 ? 1 : (props->duckDificulty == 4) ? 2 : 3;

		return props;
	}
	static DuckProperties* GenerateDuckPropertiesAccordingToDifficulty(Duck::Difficulties difficulty)
	{
		return GenerateDuckPropertiesAccordingToLevel(static_cast<int>(difficulty) * 2);
	}

private:
	CollisionUtils::CollInfo GetCollisionInfo();

	void GenerateBodyComponents();

	void UpdateAnimation(float deltaTime);
	void UpdatePosition(float deltaTime);
	void UpdateModelMatrix();
	void UpdateWingsAnimation(float);

	void CollisionDetectAndAct();

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

	Timer m_aliveTimer;
	Timer m_shotAnimationTimer;
	Timer m_resetTimer;

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

	int m_modelOrientation = 1;
	int m_nuVreauSaFaAsta = 1;

	int m_nrOfShots = 0;
	bool m_gotShot = false;
	bool m_isDead = false;
	bool m_IsFree = false;

	bool m_waitingForReset = false;

	int m_currentPositionChange = 1;

	bool m_idle = true;

	std::unique_ptr<DuckProperties> m_props;
	std::unique_ptr<ShotAnmimation> m_shotAnimation;
};