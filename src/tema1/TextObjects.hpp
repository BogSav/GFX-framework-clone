#pragma once

#include "Colors.hpp"
#include "Geometry/Rectangle.hpp"
#include "Geometry/Utilities.hpp"
#include "components/simple_scene.h"
#include "components/text_renderer.h"

class TextObject
{
public:
	TextObject() = delete;
	TextObject(WindowObject* window, glm::ivec2 resolution)
		: m_resolution(resolution), m_window(window)
	{
		TextRendererSetup();
	}

private:
	void TextRendererSetup()
	{
		m_textRenderer =
			new gfxc::TextRenderer(m_window->props.selfDir, m_resolution.x, m_resolution.y);
		m_textRenderer->Load(
			PATH_JOIN(m_window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);
	}

protected:
	gfxc::TextRenderer* m_textRenderer;
	glm::ivec2 m_resolution;
	WindowObject* m_window;
};

class Menu : public TextObject
{
public:
	Menu() = delete;
	Menu(
		WindowObject* window,
		glm::ivec2 resolution,
		Color textColor,
		float scale,
		Shader* shader,
		const gfxc::Camera* camera)
		: TextObject(window, resolution),
		  m_textColor(textColor),
		  m_scale(scale),
		  m_camera(camera),
		  m_shader(shader)
	{
		m_textBox = std::make_unique<Rectangle>(
			"text-box",
			glm::vec2{resolution.x / 2 - 150, resolution.y / 2 - 75},
			300.f,
			150.f,
			Colors::RED,
			0.f,
			true);
	}

public:
	void RenderMenu()
	{
		m_textRenderer->RenderText(
			"START",
			m_resolution.x / 2.f - 65.f,
			m_resolution.y / 2.f - 20.f,
			m_scale,
			m_textColor);

		glm::mat3 modelMatrix = glm::mat3(1);
		m_textBox->Render(m_shader, modelMatrix, m_camera);
	}

	bool CheckForStartClick(int x, int y)
	{
		if (m_textBox->GetBoundingBox().IsInside({x, y}))
		{
			m_window->HidePointer();
			return true;
		}
		return false;
	}

private:
	std::unique_ptr<Rectangle> m_textBox;

	Shader* m_shader;
	const gfxc::Camera* m_camera;

	Color m_textColor;
	float m_scale;
};

class LevelHandler : public TextObject
{
public:
	LevelHandler() = delete;
	LevelHandler(WindowObject* window, glm::ivec2 resolution, Color textColor, float scale)
		: TextObject(window, resolution), m_textColor(textColor), m_scale(scale)
	{
		m_difficulties[1] = "Easy";
		m_difficulties[2] = "Easy again";
		m_difficulties[3] = "Ehh..medium";
		m_difficulties[4] = "Medium again, let's see";
		m_difficulties[5] = "It gets harder ;)";
		m_difficulties[6] = "You start having surprises, right?";
		m_difficulties[7] = "Ok..you got here...";
		m_difficulties[8] = "I think you are rly good";
		m_difficulties[9] = "So you want more?";
		m_difficulties[10] = "One more to go!";
	}

public:
	void RenderLevelStatus(int currentLevel, int nrOfLevels)
	{
		m_textRenderer->RenderText(
			"Level: " + std::to_string(currentLevel) + " / " + std::to_string(nrOfLevels) + " - "
				+ m_difficulties[currentLevel],
			8.f,
			8.f,
			m_scale,
			m_textColor);
	}

private:
	std::unordered_map<int, std::string> m_difficulties;

	Color m_textColor;
	float m_scale;
};

class GameOver : public TextObject
{
public:
	GameOver() = delete;
	GameOver(WindowObject* window, glm::ivec2 resolution, Color textColor, float scale)
		: TextObject(window, resolution), m_textColor(textColor), m_scale(scale)
	{
	}

public:
	void RenderGameOver(bool gameWon)
	{
		m_textRenderer->RenderText(
			"GAME OVER",
			m_resolution.x / 2.f - 100.f,
			m_resolution.y / 2.f - 20.f,
			m_scale,
			m_textColor);
		if (gameWon)
			m_textRenderer->RenderText(
				"YOU WON",
				m_resolution.x / 2.f - 80.f,
				m_resolution.y / 2.f + 60.f,
				m_scale,
				m_textColor);
		else
			m_textRenderer->RenderText(
				"YOU LOST",
				m_resolution.x / 2.f - 80.f,
				m_resolution.y / 2.f + 60.f,
				m_scale,
				m_textColor);
	}

private:
	Color m_textColor;
	float m_scale;
};