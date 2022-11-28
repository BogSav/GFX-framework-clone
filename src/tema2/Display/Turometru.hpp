#pragma once

#include "2dCircle.hpp"
#include "2dRectangle.hpp"
#include "tema2/TextObjects/TextObjects.hpp"
#include "tema2/Utilities/Transformations.hpp"

#include <string>

class Turometru
{
public:
	Turometru(const WindowObject* window)
	{
		m_turometru = std::make_unique<Circle2d>(glm::vec2{175.f, 0.f}, 175.f, 20, m_turometruColor);
		m_indicator = std::make_unique<Rectangle2d>(glm::vec2{-5.f, 0.f}, 7.f, 74.f, m_indicatorColor);

		m_textRender = std::make_unique<TextObject>(window, "Algerian.ttf");
	}

	void Render(const Shader* shader, const glm::mat3& transformMatrix) const
	{
		m_turometru->Render(shader, transformMatrix, glm::mat3(1));

		glClear(GL_DEPTH_BUFFER_BIT);
		m_textRender->Render("0", 22.f, 659.f, 1.2f, m_textColor);
		m_textRender->Render("85", 66.f, 600.f, 1.2f, m_textColor);
		m_textRender->Render("170", 153.f, 572.f, 1.2f, m_textColor);
		m_textRender->Render("255", 236.f, 600.f, 1.2f, m_textColor);
		m_textRender->Render("340", 296.f, 659.f, 1.2f, m_textColor);

		m_indicator->Render(
			shader,
			transformMatrix,
			utils::Translate2d(175.f, 50.f) * utils::Rotate2d(GetAngleAccordingToSpeed(m_speed))
				* glm::mat3(1));

		m_textRender->Render(
			std::to_string(static_cast<int>(std::round(m_speed))) + " km/h", 62.f, 695.f, 1.2f);

		m_textRender->Render(
			m_speed == 0 ? "Liber" : std::to_string(m_gear), 218.f, 695.f, 1.2f);

	}
	void UpdateSpeed(const float& speed) { m_speed = speed; }
	void UpdateGear(const int& gear) { m_gear = gear; }

private:
	float GetAngleAccordingToSpeed(float speed) const
	{
		if (speed > 170)
			return -(
				std::numbers::pi_v<float> / 2.f / 170.f * speed - std::numbers::pi_v<float> / 2.f);
		else
			return std::numbers::pi_v<float> / 2.f
				- std ::numbers::pi_v<float> / 2.f / 170.f * speed;
	}

private:
	std::unique_ptr<Rectangle2d> m_indicator;
	Color m_indicatorColor = Colors::White;

	std::unique_ptr<Circle2d> m_turometru;
	Color m_turometruColor = Colors::Black;

	std::unique_ptr<TextObject> m_textRender;
	Color m_textColor = Colors::White;

	float m_speed;
	int m_gear;
};