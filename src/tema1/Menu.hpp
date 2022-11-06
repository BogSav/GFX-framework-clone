#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"

#include "Colors.hpp"
#include "Geometry/Utilities.hpp"
#include "Geometry/Rectangle.hpp"

class Menu
{
public:
    Menu() = delete;
    Menu(
        WindowObject* window, 
        glm::ivec2 resolution, 
        Color textColor, 
        float scale)
        :
        m_textColor(textColor),
        m_scale(scale),
        m_resolution(resolution)
    {
        SetMenu(window, resolution);

        m_textBox = std::make_unique<Rectangle>(
            "text-box",
            glm::vec2{
                resolution.x / 2 - 150,
                resolution.y / 2 - 75},
            300.f,
            150.f, 
            Colors::RED, 
            0.f, 
            true);
    }
public:
	void RenderMenu(Shader* shader, const gfxc::Camera* camera)
	{
        textRenderer->RenderText("START", 
            m_resolution.x / 2.f - 65.f, 
            m_resolution.y / 2.f - 20.f,
            m_scale, 
            m_textColor);

        glm::mat3 modelMatrix = glm::mat3(1);
        m_textBox->Render(shader, modelMatrix, camera);
	}
    void RenderScore(Shader* shader, const gfxc::Camera* camera)
    {
        textRenderer->RenderText("START",
            m_resolution.x / 2.f - 65.f,
            m_resolution.y / 2.f - 20.f,
            m_scale,
            m_textColor);

        glm::mat3 modelMatrix = glm::mat3(1);
        m_textBox->Render(shader, modelMatrix, camera);
    }

    bool CheckForStartClick(int x, int y, WindowObject* window)
    {
        if (m_textBox->GetBoundingBox().IsInside({ x, y }))
        {
            window->HidePointer();
            return true;
        }
        return false;
    }
private:
    void SetMenu(WindowObject* window, glm::ivec2 resolution)
    {
        textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
        textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);
    }
private:
    std::unique_ptr<Rectangle> m_textBox;

	gfxc::TextRenderer* textRenderer;
    glm::ivec2 m_resolution;
    Color m_textColor;
    float m_scale;
};