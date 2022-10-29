#pragma once

#include "components/simple_scene.h"

class Game : public gfxc::SimpleScene
{
public:
    Game();
    ~Game();

    void Init() override;

private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
};

