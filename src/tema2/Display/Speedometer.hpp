#pragma once

#include "2dCircle.hpp"
#include "2dPolygon.hpp"
#include "tema2/TextComponents/TextEngine.hpp"
#include "tema2/Utilities/Utilities.hpp"

#include <string>

class Speedometer
{
public:
	Speedometer() = delete;
	Speedometer(
		const WindowObject* window,
		Color indicatorColor = Colors::White,
		Color textColor = Colors::White,
		Color speedometerBackgroundColor = Colors::DarkGray,
		const std::string fontName = "Algerian.ttf")
		: m_indicatorColor(indicatorColor),
		  m_textColor(textColor),
		  m_speedometerBackgroundColor(speedometerBackgroundColor)
	{
		m_speedometerBackground = std::make_unique<Circle2d>(
			glm::vec2{175.f, 0.f}, 175.f, 20, m_speedometerBackgroundColor);
		m_indicator =
			std::make_unique<Polygon2d>(glm::vec2{-5.f, 0.f}, 7.f, 74.f, m_indicatorColor);

		m_textEngine = std::make_unique<TextEngine>(window, fontName);
	}

	void Render(
		const Shader* const shader,
		const glm::mat3& transformMatrix,
		const glm::mat3& modelMatrix) const
	{
		m_speedometerBackground->Render(shader, transformMatrix, modelMatrix);

		glClear(GL_DEPTH_BUFFER_BIT);
		m_textEngine->Render("0", 22.f, 659.f, 1.2f, m_textColor);
		m_textEngine->Render("85", 66.f, 600.f, 1.2f, m_textColor);
		m_textEngine->Render("170", 153.f, 572.f, 1.2f, m_textColor);
		m_textEngine->Render("255", 236.f, 600.f, 1.2f, m_textColor);
		m_textEngine->Render("340", 296.f, 659.f, 1.2f, m_textColor);

		m_indicator->Render(
			shader,
			transformMatrix,
			utils::Translate2d(175.f, 50.f) * utils::Rotate2d(GetAngleAccordingToSpeed(m_speed))
				* glm::mat3(1));

		m_textEngine->Render(
			std::to_string(static_cast<int>(std::round(m_speed))) + " km/h", 62.f, 695.f, 1.2f);

		m_textEngine->Render(m_speed == 0 ? "Liber" : std::to_string(m_gear), 218.f, 695.f, 1.2f);
	}

	void UpdateSpeed(const float& speed) { m_speed = speed; }
	void UpdateGear(const int& gear) { m_gear = gear; }

private:
	float GetAngleAccordingToSpeed(float speed) const
	{
		float piOver2 = std::numbers::pi_v<float> / 2.f;
		if (speed > 170)
			return -(piOver2 / 170.f * speed - piOver2);
		else
			return piOver2 - piOver2 / 170.f * speed;
	}

private:
	std::unique_ptr<Polygon2d> m_indicator;
	const Color m_indicatorColor;

	std::unique_ptr<Circle2d> m_speedometerBackground;
	const Color m_speedometerBackgroundColor;

	std::unique_ptr<TextEngine> m_textEngine;
	const Color m_textColor;

	float m_speed = 0.f;
	int m_gear = 0;
};