#pragma once

#include "Geometry/GeometryObject.hpp"
#include "Object.hpp"

class ShotAnmimation : public Object
{
public:
	ShotAnmimation(
		TranformUtils::LogicSpace logicSpace,
		TranformUtils::ViewportSpace viewPort,
		Shader* shader,
		const gfxc::Camera* camera,
		float scalingSpeed,
		Color color = Colors::RED)
		: Object(logicSpace, viewPort, shader, camera),
		  m_color(color),
		  m_scalingSDpeed(scalingSpeed)
	{
		m_components.emplace(
			"bottom-left",
			new Rectangle(
				"bottom - left",
				{0.1f, 0},
				{0, 0.1f},
				{0.35f, 0.45f},
				{0.45f, 0.35f},
				Colors::RED,
				0.8f));
		m_components.emplace(
			"top-left",
			new Rectangle(
				"top-left",
				{0.35f, 0.55f},
				{0, 0.9f},
				{0.1, 1.f},
				{0.45f, 0.65f},
				Colors::RED,
				0.8f));
		m_components.emplace(
			"bottom-right",
			new Rectangle(
				"bottom-right",
				{0.9f, 0},
				{0.55f, 0.35f},
				{0.65f, 0.45f},
				{1.f, 0.1f},
				Colors::RED,
				0.8f));
		m_components.emplace(
			"top-right",
			new Rectangle(
				"top-right",
				{0.65f, 0.55f},
				{0.55f, 0.65f},
				{0.9f, 1.f},
				{1.f, 0.9f},
				Colors::RED,
				0.8f));
	}

	void UpdateAndRender(glm::mat3 modelMatrix, float deltaTime)
	{
		m_scale -= m_scalingSDpeed * deltaTime;

		this->UpdateModelMatrix();
		m_modelMatrix = modelMatrix * m_modelMatrix;

		Object::Render();
	}

	void ResetShotAnimation() { m_scale = 1.f; }

private:
	void UpdateModelMatrix()
	{
		m_modelMatrix = glm::mat3(1);
		m_modelMatrix *= TranformUtils::Translate(0.5f, 0.5f);
		m_modelMatrix *= TranformUtils::Scale(m_scale, m_scale);
		m_modelMatrix *= TranformUtils::Translate(-0.5f, -0.5f);
	}

private:
	Color m_color;
	float m_scale = 1.4f;
	float m_scalingSDpeed = 0.1f;
};