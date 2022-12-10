#pragma once

#include "tema2/Utilities/Camera.hpp"
#include "tema2/Physics/Engine.hpp"

#include "tema2/Display/Speedometer.hpp"
#include "tema2/Lighting/HeadLight.hpp"
#include "tema2/GameComponents/Track.hpp"
#include "tema2/GameComponents/NPC.hpp"

class Car
{
public:
	class PositionObeserver
	{
	public:
		PositionObeserver() = default;
		PositionObeserver(int startPt, size_t totalNrOfPts);
		void UpdateCurrentPoint(const Track* track, const Car* car);

		float GetPercent();
		int GetTotalNrOfQuads() const;
		int GetStartPoint() const;
		int GetTrackPointsNr() const;
	private:
		int m_currentPoint;
		int m_startPoint;
		size_t m_trackPointsNr;

		int m_prevQuad;
		int m_totalNrOfQuads;
	};

	class PlaceTracker
	{
	public:
		PlaceTracker() = default;

		void InsertNPC(const NPC* npc);
		void UpdatePlace(int nrOfQuads, int startQuad, int totalNrOfPts);

		std::pair<int, size_t> GetPlaceAndNrOfNPCs();
	private:
		std::vector<const NPC*> m_npcs;
		int m_place;
	};

public:
	Car() = delete;
	Car(const WindowObject*, const Shader* const);

	void InitializeObserver(const Track*);

	void Render() const;
	void Render(CustomCamera* const camera, const Shader* const) const;

	void Update(double);
	void UpdateOrientation(float);

	void Accelerate();
	void Brake();
	void InertialDecceleration();

	void RestoreLastState();

	void PrintData();

	const glm::vec3& GetPosition() const { return m_position; }
	const std::shared_ptr<Speedometer>& GetTurometru() const { return m_speedometer; }
	const std::shared_ptr<CustomCamera>& GetCamera() const { return m_camera; };
	std::shared_ptr<PositionObeserver>& GetObserver() { return m_observer; }
	std::shared_ptr<PlaceTracker>& GetPlaceTracker() { return m_placeTracker; }

	const HeadLight* GetLeftHeadLight() const { return m_headLightLeft.get(); }
	const HeadLight* GetRightHeadLight() const { return m_headLightRight.get(); }

	friend class CollisionEngine;

private:
	void ComputeModelMatrix();
	void UpdateLastParameters();

private:
	const Shader* const m_shader;
	std::shared_ptr<CustomCamera> m_camera;
	std::unique_ptr<CustomCamera> m_lastCamera;

	std::unique_ptr<physics::Engine> m_engine;
	std::unique_ptr<physics::GearBox> m_gearBox;
	std::shared_ptr<Speedometer> m_speedometer;
	std::shared_ptr<PositionObeserver> m_observer;
	std::unique_ptr<PositionObeserver> m_lastObserver;
	std::shared_ptr<PlaceTracker> m_placeTracker;

	std::unique_ptr<HeadLight> m_headLightLeft;
	std::unique_ptr<HeadLight> m_headLightRight;

	std::unique_ptr<Mesh> m_mesh;

	glm::vec3 m_position;
	glm::vec3 m_direction;
	float m_angleOrientation;
	glm::vec3 m_scale;

	glm::vec3 m_lastPosition;
	glm::vec3 m_lastDirection;
	float m_lastOrientationAngle;

	DTimer m_updateLastParametersTimer;

	const float m_distanceFromCamera;
	const float m_stirringAngularSpeed;

	glm::mat4 m_modelMatrix;
};