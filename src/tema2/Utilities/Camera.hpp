#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

class CustomCamera
{
public:
	CustomCamera() = default;

	CustomCamera(
		const glm::vec3& position,
		const glm::vec3& center,
		const glm::vec3& up,
		const float aspectRatio)
	{
		Set(position, center, up, aspectRatio);
	}

	void Set(
		const glm::vec3& position,
		const glm::vec3& center,
		const glm::vec3& up,
		const float aspectRatio)
	{
		this->position = position;
		this->forward = glm::normalize(center - position);
		this->right = glm::cross(forward, up);
		this->up = glm::cross(right, forward);
		this->distanceToTarget = glm::length(center - position);

		this->projectionMatrix = glm::perspective(m_fov, aspectRatio, m_zNear, m_Zfar);
	}

	void MoveForward(float distance)
	{
		glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
		position += dir * distance;
	}

	// Translate functions
	void TranslateForward(float distance)
	{
		position = position + distance * glm::normalize(forward);
	}
	void TranslateUpward(float distance) { position += glm::vec3{0,1,0} * distance; }
	void TranslateRight(float distance)
	{
		position += glm::cross({0, 1, 0}, glm::cross(glm::normalize(right), {0, 1, 0})) * distance;
	}

	// Rotation functions - first person
	void RotateFirstPerson_OX(float angle)
	{
		forward =
			glm::normalize(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1));
		up = glm::normalize(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(up, 1));
	}
	void RotateFirstPerson_OY(float angle)
	{
		right =
			glm::normalize(glm::rotate(glm::mat4(1.0f), angle, {0, 1, 0}) * glm::vec4(right, 1));
		forward =
			glm::normalize(glm::rotate(glm::mat4(1.0f), angle, {0, 1, 0}) * glm::vec4(forward, 1));

		up = glm::normalize(glm::cross(right, forward));
	}

	// Rotation functions - third person
	void RotateThirdPerson_OX(float angle)
	{
		glm::mat4 modelMatrix = glm::mat4(1);

		modelMatrix = glm::translate(glm::mat4(1), glm::normalize(forward) * distanceToTarget);
		position = glm::vec3(modelMatrix * glm::vec4(position, 1));

		RotateFirstPerson_OX(angle);

		modelMatrix = glm::translate(glm::mat4(1), -glm::normalize(forward) * distanceToTarget);
		position = glm::vec3(modelMatrix * glm::vec4(position, 1));
	}
	void RotateThirdPerson_OY(float angle)
	{
		glm::mat4 modelMatrix = glm::mat4(1);

		modelMatrix = glm::translate(glm::mat4(1), glm::normalize(forward) * distanceToTarget);
		position = glm::vec3(modelMatrix * glm::vec4(position, 1));

		RotateFirstPerson_OY(angle);

		modelMatrix = glm::translate(glm::mat4(1), -glm::normalize(forward) * distanceToTarget);
		position = glm::vec3(modelMatrix * glm::vec4(position, 1));
	}

	// Getters for components
	glm::mat4 GetViewMatrix() const { return glm::lookAt(position, position + forward, up); }
	glm::mat4 GetProjectionMatrix() const { return this->projectionMatrix; }

	glm::vec3 GetTargetPosition() const { return position + forward * distanceToTarget; }

	glm::vec3& GetPosition() { return position; };

public:
	float distanceToTarget;
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	glm::mat4 projectionMatrix;

	float m_fov = RADIANS(30);
	float m_zNear = 0.1;
	float m_Zfar = 500;
};