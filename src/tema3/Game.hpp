#pragma once

#include "components/simple_scene.h"
#include "tema3/Utilities/Camera.hpp"

#include "tema3/GameComponents/Field.hpp"

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

	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

	void CreateShaders();

private:
	std::unique_ptr<CustomCamera> m_camera;

	std::unique_ptr<GameComponent> m_field;
	std::unique_ptr<GameComponent> m_player;
};