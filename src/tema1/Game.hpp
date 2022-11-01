#pragma once

#include "components/simple_scene.h"

#include "Object.hpp"
#include "2dTransformations.hpp"
#include "Menu.hpp"
#include "Duck.hpp"

class Game : public gfxc::SimpleScene
{
public:
    Game();
    ~Game();

    void Init() override;

private:
    void FrameStart() override;
    void RenderScene(glm::mat3, float);
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;

private:
    TranformUtils::LogicSpace m_logicSpace;
    TranformUtils::ViewportSpace m_viewPortSpace;
    Object* obj;
    Menu* m_menu;
    Duck* m_duck;

    glm::vec2 m_position = { 5, 5 };
    glm::vec2 m_direction = { 0,0 };
    float m_speed = 10.;

    bool m_inGame = false;
    bool da = false;
    float timmer = 0;
};

