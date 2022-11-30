#pragma once

#include "tema2/Utilities/Camera.hpp"
#include "tema2/Physics/Engine.hpp"

#include "tema2/Display/Turometru.hpp"

class Masina
{
public:
	Masina(const WindowObject* const, const Shader* const);

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
	const Turometru* const GetTurometru() const { return m_turometru.get(); }
	const CustomCamera* const GetCamera() const { return m_camera.get(); };

	friend class CollisionEngine;

private:
	void ComputeModelMatrix();
	void UpdateLastParameters();

private:
	const Shader* const m_shader;
	std::unique_ptr<CustomCamera> const m_camera;
	std::unique_ptr<CustomCamera> const m_lastCamera;

	std::unique_ptr<physics::Engine> m_engine;
	std::unique_ptr<GearBox> m_gearBox;
	std::unique_ptr<Turometru> m_turometru;

	std::unique_ptr<Mesh> m_mesh;

	glm::vec3 m_position;
	glm::vec3 m_direction;
	float m_angleOrientation;
	glm::vec3 m_scale;

	glm::vec3 m_lastPosition;
	glm::vec3 m_lastDirection;
	float m_lastOrientationAngle;

	DTimer m_updateLastParametersTimer;

	float m_distanceFromCamera;

	float m_stirringAngularSpeed = RADIANS(45);

	glm::mat4 m_modelMatrix;
};