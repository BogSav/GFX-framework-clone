#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"

#include "tema2/Utilities/Colors.hpp"

class TextObject
{
public:
	TextObject() = delete;
	TextObject(const WindowObject* window, const std::string& fontName = "Hack-Bold.ttf", const int fontSize = 18)
		: m_window(window)
	{
		m_textRenderer = new gfxc::TextRenderer(
			m_window->props.selfDir, m_window->GetResolution().x, m_window->GetResolution().y);
		m_textRenderer->Load(
			PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, fontName), fontSize);
	}

	void Render(
		const std::string& text,
		const float posX,
		const float posY,
		const float scale = 1.f,
		Color color = Colors::White)
	{
		m_textRenderer->RenderText(text, posX, /*m_window->GetResolution().y -*/ posY, scale, color());
	}

protected:
	gfxc::TextRenderer* m_textRenderer;
	const WindowObject* m_window;
};