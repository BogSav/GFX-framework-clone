#include "tema1/Game.hpp"

#include "Triangle.hpp"
#include "Rectangle.hpp"
#include "Circle.hpp"

#include <vector>
#include <iostream>


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
    m_viewPortSpace.SetY(0);
    m_viewPortSpace.SetHeight(resolution.y);
    m_viewPortSpace.SetWidth(resolution.x);

    m_logicSpace.SetX(0);
    m_logicSpace.SetY(0);
    m_logicSpace.SetHeight(20);
    m_logicSpace.SetWidth(20);

    m_duck = new Duck(m_logicSpace, m_viewPortSpace);
    m_menu = new Menu(window, window->GetResolution());
}


void Game::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    glViewport(0, 0, resolution.x, resolution.y);
}

void Game::RenderScene(float deltaTimeSeconds)
{
    m_duck->Update(deltaTimeSeconds);
    m_duck->Render(shaders["VertexColor"], GetSceneCamera());
}

void Game::Update(float deltaTimeSeconds)
{
    m_duck->CollisionDetectAndAct();
    RenderScene(deltaTimeSeconds);

   /* if (!m_inGame)
        m_menu->RenderMenu(deltaTimeSeconds);*/
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
    if (m_duck->GotShot({ mouseX, m_viewPortSpace.GetUpperY() - mouseY }))
        std::cout << "asdfasdf" << std::endl;
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}
