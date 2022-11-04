#pragma once

#include "components/simple_scene.h"

#include "Object.hpp"
#include "Utilities.hpp"
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
    void RenderScene(float);
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;

    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

private:
    TranformUtils::LogicSpace m_logicSpace;
    TranformUtils::ViewportSpace m_viewPortSpace;
    Object* obj;
    Menu* m_menu;
    Duck* m_duck;

    bool m_inGame = false;
};

