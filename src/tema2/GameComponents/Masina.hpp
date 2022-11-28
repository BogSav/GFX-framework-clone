#pragma once

#include "tema2/Utilities/Camera.hpp"
#include "tema2/Physics/Engine.hpp"

#include "components/simple_scene.h"
#include "core/engine.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "tema2/Display/Turometru.hpp"

class Masina
{
public:
	Masina(const WindowObject*, Shader*);

	void Render() const;
	void Render(const CustomCamera* const camera, const Shader*) const;

	void Update(double);
	void UpdateOrientation(float);

	void Accelerate();
	void Brake();
	void InertialDecceleration();

	void RestoreLastState();

	void PrintData();

	const Turometru* GetTurometru() { return m_turometru.get(); }
	const glm::vec3 GetPosition() const { return m_position; }
	CustomCamera* GetCamera() { return m_camera; };

	friend class CollisionEngine;

private:
	void ComputeModelMatrix();
	void UpdateLastParameters();

private:
	Shader* m_shader;
	CustomCamera* m_camera;
	CustomCamera* m_lastCamera;

	std::unique_ptr<physics::Engine> m_engine;
	std::unique_ptr<GearBox> m_gearBox;

	std::unique_ptr<Mesh> m_mesh;

	glm::vec3 m_position;
	glm::vec3 m_direction;
	float m_angleOrientation;
	glm::vec3 m_scale;

	glm::vec3 m_lastPosition;
	glm::vec3 m_lastDirection;
	float m_lastOrientationAngle;

	DTimer m_updateLastParametersTimer;

	float m_distanceFromCamera = 3;

	float m_stirringAngularSpeed = RADIANS(45);

	glm::mat4 m_modelMatrix;

	std::unique_ptr<Turometru> m_turometru;
};