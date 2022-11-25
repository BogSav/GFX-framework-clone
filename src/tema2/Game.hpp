#pragma once

#include "components/simple_scene.h"

#include "GameComponents/Pista.hpp"

#include "Utilities/Camera.hpp"

#include "Physics/Engine.hpp"

#include "tema2/GameComponents/Masina.hpp"

class Game : public gfxc::SimpleScene
{
public:
	Game();
	~Game();

	void Init() override;

private:
	void FrameStart() override;
	void RenderAndUpdateGameComponents(float);
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;

	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

	void RenderGameComponents();

private:
	std::vector<std::unique_ptr<GameComponent>> m_components;

	CustomCamera* m_camera;

	std::unique_ptr<Masina> m_car;
};