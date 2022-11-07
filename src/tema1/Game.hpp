#pragma once

#include "Bullets.hpp"
#include "Crosshair.hpp"
#include "Duck.hpp"
#include "Field.hpp"
#include "Geometry/Utilities.hpp"
#include "Hearts.hpp"
#include "Object.hpp"
#include "ProgressHandler.hpp"
#include "TextObjects.hpp"
#include "components/simple_scene.h"

class Game : public gfxc::SimpleScene
{
public:
	Game();
	~Game();

	void Init() override;

private:
	void FrameStart() override;
	void RenderGameSceneAndComponents(float);
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

	std::unique_ptr<Menu> m_menu;
	std::unique_ptr<LevelHandler> m_levelHandler;
	std::unique_ptr<GameOver> m_gameOverHandler;


	std::unique_ptr<Duck> m_duck;
	std::unique_ptr<Crosshair> m_crosshair;
	std::unique_ptr<Field> m_field;
	std::unique_ptr<Hearts> m_hearts;
	std::unique_ptr<ProgressHandler> m_handler;
	std::unique_ptr<Bullets> m_bullets;

	std::unique_ptr<Rectangle> m_ocassionalBBox;

	bool m_inGame = false;
	bool m_gameOver = false;

	glm::vec4 m_backGroundColor = {0, 0, 0, 1};

	float m_timeBetweenDucks = 0;
	float m_maxTimeBetweenDucks = 1.5;

	float m_shotAnimationDuration = 0.1f;
	float m_shotAnimationTime = 0;
	int m_shotAnimationActive = 0;
	bool m_justGotShot = false;

	int m_currentLevel = 3;
};
