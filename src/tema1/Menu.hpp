#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"

class Menu
{
public:
    Menu()
        :
        kTextColor(NormalizedRGB(166, 172, 205))
    {
        textRenderer = nullptr;
    }
public:
	void SetMenu(WindowObject* window, glm::ivec2 resolution)
	{
		textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
		textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);
	}

	void RenderMenu(float deltyTime)
	{
        const float kTopY = 10.f;
        const float kRowHeight = 25.f;

        int rowIndex = 0;
        std::string polygonModeText = "";

        textRenderer->RenderText("Keys", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("space : toggle solid/wireframe/points", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("    z : draw in wireframe mode", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("    x : draw in points mode", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("    c : draw in solid mode", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);

        textRenderer->RenderText("Current mode: " + polygonModeText, 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
	}
private:
	gfxc::TextRenderer* textRenderer;
    const glm::vec3 kTextColor;
};