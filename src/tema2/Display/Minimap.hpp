#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "tema2/GameComponents/Masina.hpp"
#include "tema2/Geometries/GeometryObject.hpp"
#include "tema2/Geometries/PlanarRectangle.hpp"
#include "tema2/Utilities/Camera.hpp"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

class MiniMap
{
public:
	MiniMap(
		const WindowObject* window,
		const glm::vec2 screenPosition,
		const float width,
		const float height,
		const float left = -200.f,
		const float bottom = -200.f)
		: m_screenPosition(screenPosition), m_width(width), m_height(height)
	{
		m_ortoCamera = std::make_unique<CustomCamera>();
		m_ortoCamera->Set(
			glm::vec3(0, 10, 0),
			glm::vec3(0, 0, 0),
			glm::vec3(-1, 0, -1),
			window->props.aspectRatio,
			true,
			left,
			-left,
			bottom,
			-bottom);

		m_carRepresentation = std::make_unique<Rectangle2d>(
			m_screenPosition + glm::vec2{GetWidth() / 2.f - 10.f, GetHeight() / 2.f - 10.f},
			20.f,
			20.f,
			Colors::Magenta);
	}

	float GetHeight() const { return m_height; }
	float GetWidth() const { return m_width; }
	float GetX() const { return m_screenPosition.x; }
	float GetY() const { return m_screenPosition.y; }

	const CustomCamera* GetCamera() const { return m_ortoCamera.get(); }

	void UpdateMinimapCameraBasedOnCarPosition(const Masina* car)
	{
		m_ortoCamera->SetPosition(car->GetPosition() + glm::vec3{0, 10, 0}, car->GetPosition());
	}

	void RenderCarRepresentation(
		const Shader* shader, const glm::mat3& transformMatrix, const glm::mat3 modelMatrix) const
	{
		m_carRepresentation->Render(shader, transformMatrix, modelMatrix);
	}

private:
	void UpdateCameraPosition(const glm::vec3& position, const glm::vec3& center)
	{
		m_ortoCamera->SetPosition(position, center);
	}

private:
	std::unique_ptr<CustomCamera> m_ortoCamera;
	std::unique_ptr<Rectangle2d> m_carRepresentation;

	glm::vec2 m_screenPosition;
	float m_width;
	float m_height;
};