#include "tema2/Game.hpp"

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
	//glm::ivec2 resolution = window->GetResolution();
	//auto camera = GetSceneCamera();
	//camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	//camera->SetPosition(glm::vec3(0, 0, 50));
	//camera->SetRotation(glm::vec3(0, 0, 0));
	//camera->Update();
	//GetCameraInput()->SetActive(false);

	{
		Shader* shader = new Shader("MyShader");
		shader->AddShader(
			PATH_JOIN(
				window->props.selfDir, SOURCE_PATH::TEMA2, "shaders", "VertexShader.glsl"),
			GL_VERTEX_SHADER);
		shader->AddShader(
			PATH_JOIN(
				window->props.selfDir, SOURCE_PATH::TEMA2, "shaders", "FragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	m_pista = std::make_unique<Pista>(shaders["MyShader"], GetSceneCamera(), 4);
}


void Game::FrameStart()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	glViewport(0, 0, resolution.x, resolution.y);
}

void Game::RenderAndUpdateGameComponents(float deltaTimeSeconds)
{

}

void Game::Update(float deltaTimeSeconds)
{
	m_pista->Render();
}

void Game::FrameEnd()
{
	DrawCoordinateSystem();
}


void Game::OnInputUpdate(float deltaTime, int mods)
{
}


void Game::OnKeyPress(int key, int mods)
{
}


void Game::OnKeyRelease(int key, int mods)
{
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}