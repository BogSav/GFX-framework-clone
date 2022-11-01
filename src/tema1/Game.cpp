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

    m_menu = new Menu();
    m_menu->SetMenu(window, window->GetResolution());

    //obj = new Triangle("tri", { 0,0 }, { 1,0 }, { 0,1 }, glm::vec3(1, 0, 0), true);
    //AddMeshToList(obj->GetMesh());

    //Object* obj2 = new Rectangle("dr", { 0,0 }, { 0, 1 }, { 2,1 }, {2, 0}, glm::vec3(1, 0, 0), true);
    //AddMeshToList(obj2->GetMesh());

    /*obj = new Circle("cer", 7, glm::vec3{ 1, 0, 0 }, 2);*/
    //ddMeshToList(obj3->GetMesh());

    m_duck = new Duck({ 0,0 });

    m_viewPortSpace.x = 0;
    m_viewPortSpace.y = 0;
    m_viewPortSpace.height = resolution.y;
    m_viewPortSpace.width = resolution.x;

    m_logicSpace.x = 0;
    m_logicSpace.y = 0;
    m_logicSpace.width = 20;
    m_logicSpace.height = 20;
}


void Game::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    glViewport(0, 0, resolution.x, resolution.y);
}

void Game::RenderScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    m_position += m_direction * m_speed * deltaTimeSeconds;

    glm::mat3 modelMatrix;

    glm::mat3 trMatrix = glm::mat3(1);
    trMatrix *= TranformUtils::Translate(m_position[0], m_position[1]);
    trMatrix *= TranformUtils::Scale(5, 5);

    modelMatrix = visMatrix * TranformUtils::Translate(m_position[0], m_position[1]);
    modelMatrix *= TranformUtils::Scale(5, 5);
    //modelMatrix *= TranformUtils::Rotate(-std::numbers::pi / 2);
    //RenderMesh2D(meshes["tri"], shaders["VertexColor"], modelMatrix);
    //obj->Render(shaders["VertexColor"], modelMatrix, GetSceneCamera());
    m_duck->Render(shaders["VertexColor"], modelMatrix, GetSceneCamera());

    if (m_inGame)
    {
        obj = new Rectangle("bbox", m_duck->GetBoundingBox(), Colors::RED);
        m_inGame = false;
    }

    if(da)
        obj->Render(shaders["VertexColor"], glm::mat3(1), GetSceneCamera());
}

void Game::Update(float deltaTimeSeconds)
{
    glm::mat3 visMatrix;
    visMatrix = glm::mat3(1);
    visMatrix *= TranformUtils::VisualizationTransf2DUnif(m_logicSpace, m_viewPortSpace);
    RenderScene(visMatrix, deltaTimeSeconds);

    timmer += deltaTimeSeconds;

    if (timmer > 5 && m_inGame == false && da == false)
    {
        m_inGame = true;
        da = true;
    }

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
    switch (key)
    {
    case GLFW_KEY_W:
        m_direction += glm::vec2{ 0, 1 };
        break;
    }
}


void Game::OnKeyRelease(int key, int mods)
{
    switch (key)
    {
    case GLFW_KEY_W:
        m_direction -= glm::vec2{ 0, 1 };
        break;
    }
}
