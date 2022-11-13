#include "tema1/Game.hpp"

#include "Geometry/Circle.hpp"
#include "Geometry/Rectangle.hpp"
#include "Geometry/Triangle.hpp"

#include <iostream>
#include <vector>


Game::Game()
{
}


Game::~Game()
{
}


void Game::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	m_viewPortSpace.SetX(0);
	m_viewPortSpace.SetY(20);
	m_viewPortSpace.SetHeight(resolution.y - 20);
	m_viewPortSpace.SetWidth(resolution.x);

	m_logicSpace.SetX(0);
	m_logicSpace.SetY(0);
	m_logicSpace.SetHeight(20);
	m_logicSpace.SetWidth(20);

	m_duck = std::make_unique<Duck>(
		m_logicSpace,
		m_viewPortSpace,
		shaders["VertexColor"],
		GetSceneCamera(),
		Duck::GenerateDuckPropertiesAccordingToDifficulty(Duck::Difficulties::BABY));

	m_duck2 = std::make_unique<Duck>(
		m_logicSpace,
		m_viewPortSpace,
		shaders["VertexColor"],
		GetSceneCamera(),
		Duck::GenerateDuckPropertiesAccordingToDifficulty(Duck::Difficulties::MEDIUM));

	m_menu = std::make_unique<Menu>(
		window,
		window->GetResolution(),
		Colors::RED,
		2.5f,
		shaders["VertexColor"],
		GetSceneCamera());

	m_gameOverHandler =
		std::make_unique<GameOver>(window, window->GetResolution(), Colors::RED, 2.5f);

	m_levelHandler =
		std::make_unique<LevelHandler>(window, window->GetResolution(), Colors::BLUE, 1.f);

	m_crosshair = std::make_unique<Crosshair>(
		m_logicSpace, m_viewPortSpace, 3.5f, shaders["VertexColor"], GetSceneCamera());

	m_field = std::make_unique<Field>(
		TranformUtils::LogicSpace{0.5, 0, 6, 2},
		TranformUtils::ViewportSpace{0, 0, resolution.x, 330},
		Colors::GREEN,
		shaders["VertexColor"],
		GetSceneCamera());

	m_hearts = std::make_unique<Hearts>(
		TranformUtils::LogicSpace{0, 0, 12, 4},
		TranformUtils::ViewportSpace{30, 40, 300, 100},
		shaders["VertexColor"],
		GetSceneCamera());

	m_progressHandler = std::make_unique<ProgressHandler>(
		TranformUtils::LogicSpace{0, 0, 5, 1},
		TranformUtils::ViewportSpace{520, 45, 200, 40},
		m_duck->GetSlaveryTime(),
		shaders["VertexColor"],
		GetSceneCamera());

	m_bullets = std::make_unique<Bullets>(
		TranformUtils::LogicSpace{0, 0, 4, 2},
		TranformUtils::ViewportSpace{1070, 30, 200, 100},
		shaders["VertexColor"],
		GetSceneCamera());
	// m_ocassionalBBox = std::make_unique<Rectangle>("bbox", m_duck->GetRawBBox(), Colors::RED,
	// true);
}


void Game::FrameStart()
{
	glClearColor(
		m_backGroundColor[0], m_backGroundColor[1], m_backGroundColor[2], m_backGroundColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	glViewport(0, 0, resolution.x, resolution.y);
}

void Game::RenderAndUpdateGameComponents(float deltaTimeSeconds)
{
	m_duck->Update(deltaTimeSeconds);
	m_duck2->Update(deltaTimeSeconds);

	m_duck->Render();
	m_duck2->Render();

	m_crosshair->Render();
	m_field->Render();
	m_hearts->Render();
	m_progressHandler->Render();
	m_bullets->Render();
	m_levelHandler->RenderLevelStatus(m_currentLevel, 10);
}

void Game::Update(float deltaTimeSeconds)
{
	// m_ocassionalBBox->Render(shaders["VertexColor"], m_duck->GetModelMatrix(), GetSceneCamera());
	if (!m_inGame)
	{
		m_menu->RenderMenu();
	}
	else if (!m_gameOver && m_currentLevel <= 10)
	{
		//JumpToLevel11();
		if (m_duck->IsDead())
		{
			m_backGroundColor = Colors::getColorPlusAlpha(Colors::DARK_RED, 1.f);
		}
		else if (m_duck->IsFree())
		{
			m_backGroundColor = Colors::getColorPlusAlpha(Colors::LIGHT_GOLD, 1.f);
		}
		else
		{
			m_progressHandler->SetProgress(m_duck->GetAliveTime());
		}

		if (m_duck->IsFreeForReset())
		{
			if (m_duck->IsFree())
			{
				m_hearts->LoseAHeart();
				if (!m_hearts->AnyHeartsLeft())
				{
					m_gameOver = true;
				}
			}
			else
			{
				m_currentLevel++;
			}

			if (m_currentLevel < 11)
			{
				m_duck.reset(new Duck(
					m_logicSpace,
					m_viewPortSpace,
					shaders["VertexColor"],
					GetSceneCamera(),
					Duck::GenerateDuckPropertiesAccordingToLevel(m_currentLevel)));

				m_bullets->ResetBullets();
				m_duck->SetActive();
					
				m_progressHandler->ResetLimit(m_duck->GetSlaveryTime());
			}
			else
			{
				m_duck.reset(new Duck(
					m_logicSpace,
					m_viewPortSpace,
					shaders["VertexColor"],
					GetSceneCamera(),
					Duck::GenerateDuckPropertiesAccordingToDifficulty(
						Duck::Difficulties::EASY)));
					
				m_duck->SetActive();
				m_duck2->SetActive();

				m_bullets->ResetBullets();
				m_progressHandler->ResetLimit(
					std::max(m_duck->GetSlaveryTime(), m_duck2->GetSlaveryTime()));
			}
			m_backGroundColor = Colors::getColorPlusAlpha(Colors::CYAN, 1.f);
		}
		RenderAndUpdateGameComponents(deltaTimeSeconds);
	}
	else if (!m_gameOver && m_currentLevel == 11)
	{
		if (m_duck->IsDead() && m_duck2->IsDead())
		{
			m_backGroundColor = Colors::getColorPlusAlpha(Colors::DARK_RED, 1.f);
		}
		else if (m_duck->IsFree() || m_duck2->IsFree())
		{
			m_duck2->SetFree();
			m_duck->SetFree();
			m_backGroundColor = Colors::getColorPlusAlpha(Colors::LIGHT_GOLD, 1.f);
		}
		else
		{
			m_progressHandler->SetProgress(m_duck->GetAliveTime());
		}

		if (m_duck->IsFreeForReset() && m_duck2->IsFreeForReset())
		{
			if (m_duck->IsFree())
			{
				m_hearts->LoseAHeart();
				if (!m_hearts->AnyHeartsLeft())
				{
					m_gameOver = true;
					return;
				}
			}
			else
			{
				m_currentLevel++;
			}

			m_duck.reset(new Duck(
				m_logicSpace,
				m_viewPortSpace,
				shaders["VertexColor"],
				GetSceneCamera(),
				Duck::GenerateDuckPropertiesAccordingToDifficulty(Duck::Difficulties::EASY)));
			m_duck2.reset(new Duck(
				m_logicSpace,
				m_viewPortSpace,
				shaders["VertexColor"],
				GetSceneCamera(),
				Duck::GenerateDuckPropertiesAccordingToDifficulty(Duck::Difficulties::MEDIUM)));
				
			m_duck->SetActive();
			m_duck2->SetActive();
				
			m_bullets->ResetBullets();
			m_progressHandler->ResetLimit(
				std::min(m_duck->GetSlaveryTime(), m_duck2->GetSlaveryTime()));
				
			m_backGroundColor = Colors::getColorPlusAlpha(Colors::CYAN, 1.f);
		}

		RenderAndUpdateGameComponents(deltaTimeSeconds);
	}
	else
	{
		m_gameOverHandler->RenderGameOver(m_currentLevel > 11);
		m_backGroundColor = Colors::getColorPlusAlpha(Colors::BLACK, 1.f);
	}
}

void Game::JumpToLevel11()
{
	m_currentLevel = 11;
	m_bullets->ResetBullets();
	m_duck.reset(new Duck(
		m_logicSpace,
		m_viewPortSpace,
		shaders["VertexColor"],
		GetSceneCamera(),
		Duck::GenerateDuckPropertiesAccordingToDifficulty(Duck::Difficulties::EASY)));
	m_duck->SetActive();
	m_duck2->SetActive();
	m_progressHandler->ResetLimit(std::max(m_duck->GetSlaveryTime(), m_duck2->GetSlaveryTime()));
}

void Game::FrameEnd()
{
	// DrawCoordinateSystem();
}


void Game::OnInputUpdate(float deltaTime, int mods)
{
}


void Game::OnKeyPress(int key, int mods)
{
	if (m_gameOver && key == GLFW_KEY_SPACE)
	{
		m_currentLevel = 1;
		m_inGame = false;
		m_gameOver = false;
		window->ShowPointer();
		m_duck.reset(new Duck(
			m_logicSpace,
			m_viewPortSpace,
			shaders["VertexColor"],
			GetSceneCamera(),
			Duck::GenerateDuckPropertiesAccordingToLevel(m_currentLevel)));
		m_duck2.reset(new Duck(
			m_logicSpace,
			m_viewPortSpace,
			shaders["VertexColor"],
			GetSceneCamera(),
			Duck::GenerateDuckPropertiesAccordingToLevel(Duck::Difficulties::BABY)));
		m_bullets->ResetBullets();
		m_hearts->ResetHearts();
		m_progressHandler->ResetLimit(m_duck->GetSlaveryTime());
	}
}


void Game::OnKeyRelease(int key, int mods)
{
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (m_inGame)
	{
		m_crosshair->UpdatePosition(mouseX, m_viewPortSpace.GetUpperY() - mouseY);
	}
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (m_inGame)
	{
		m_bullets->ShotBullet();

		m_duck->Shot(m_crosshair->GetBoundingBox());
		m_duck2->Shot(m_crosshair->GetBoundingBox());

		if (m_currentLevel != 11)
		{
			if (m_bullets->GetRemainingBullets() < m_duck->GetRemainingRequiredShots())
			{
				m_duck->SetFree();
				m_duck->SetTimeServed();
			}
		}
		else
		{
			if (m_bullets->GetRemainingBullets()
				< (m_duck->GetRemainingRequiredShots() + m_duck2->GetRemainingRequiredShots()))
			{
				m_duck->SetFree();
				m_duck->SetTimeServed();

				m_duck2->SetFree();
				m_duck2->SetTimeServed();
			}
		}
	}
	else
	{
		if (m_menu->CheckForStartClick(mouseX, m_viewPortSpace.GetUpperY() - mouseY))
		{
			m_inGame = true;
			m_crosshair->UpdatePosition(mouseX, m_viewPortSpace.GetUpperY() - mouseY);
			m_backGroundColor = Colors::getColorPlusAlpha(Colors::CYAN, 1.f);
			m_duck->SetActive();
		}
	}
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}
