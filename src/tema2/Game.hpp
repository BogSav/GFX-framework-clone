#pragma once

#include "components/simple_scene.h"

#include "tema2/GameComponents/Track.hpp"

#include "Utilities/Camera.hpp"

#include "Physics/Engine.hpp"

#include "tema2/GameComponents/Car.hpp"

#include "tema2/GameComponents/Field.hpp"
#include "tema2/Display/Screen.hpp"

#include "tema2/Display/Minimap.hpp"

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
	void RenderMinimap();
	void RenderScreenObjects();

	void UpdateGameComponents(float);

private:
	std::vector<std::unique_ptr<GameComponent>> m_components;

	CustomCamera* m_camera;

	std::unique_ptr<Car> m_car;
	GeometryObject* obj;

	DTimer frametimer;

	std::unique_ptr<ScreenElements> m_screen;
	std::unique_ptr<MiniMap> m_minimap;
};