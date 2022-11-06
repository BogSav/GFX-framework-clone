#include "tema1/Game.hpp"

#include "Geometry/Triangle.hpp"
#include "Geometry/Rectangle.hpp"
#include "Geometry/Circle.hpp"

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
    m_viewPortSpace.SetY(20);
    m_viewPortSpace.SetHeight(resolution.y - 20);
    m_viewPortSpace.SetWidth(resolution.x);

    m_logicSpace.SetX(0);
    m_logicSpace.SetY(0);
    m_logicSpace.SetHeight(20);
    m_logicSpace.SetWidth(20);

    m_menu = std::make_unique<Menu>(
        window, 
        window->GetResolution(), 
        Colors::RED, 
        2.5f);

    m_duck = std::make_unique<Duck>(
        m_logicSpace, 
        m_viewPortSpace);
    
    m_crosshair = std::make_unique<Crosshair>(
        m_logicSpace, 
        m_viewPortSpace, 
        3.5f);

    m_field = std::make_unique<Field>(
        TranformUtils::LogicSpace{0.5, 0, 6, 2},
        TranformUtils::ViewportSpace{0, 0, resolution.x, 330}, 
        Colors::GREEN);
    
    m_hearts = std::make_unique<Hearts>(
        TranformUtils::LogicSpace{0, 0, 12, 4},
        TranformUtils::ViewportSpace{30, 650, 300, 100});

    m_handler = std::make_unique<ProgressHandler>(
        TranformUtils::LogicSpace{ 0, 0, 5, 1 },
        TranformUtils::ViewportSpace{ 520, 650, 200, 40 },
        m_duck->GetSlaveryTime());

    m_bullets = std::make_unique<Bullets>(
        TranformUtils::LogicSpace{ 0, 0, 4, 2 },
        TranformUtils::ViewportSpace{ 1000, 30, 200, 100});
    //m_ocassionalBBox = std::make_unique<Rectangle>("bbox", m_duck->GetRawBBox(), Colors::RED, true);
}


void Game::FrameStart()
{
    glClearColor(m_backGroundColor[0], m_backGroundColor[1], m_backGroundColor[2], m_backGroundColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    glViewport(0, 0, resolution.x, resolution.y);
}

void Game::RenderGameSceneAndComponents(float deltaTimeSeconds)
{
    m_duck->Render(shaders["VertexColor"], GetSceneCamera());
    m_crosshair->Render(shaders["VertexColor"], GetSceneCamera());
    m_field->Render(shaders["VertexColor"], GetSceneCamera());
    m_hearts->Render(shaders["VertexColor"], GetSceneCamera());
    m_handler->Render(shaders["VertexColor"], GetSceneCamera());
    m_bullets->Render(shaders["VertexColor"], GetSceneCamera());
}

void Game::Update(float deltaTimeSeconds)
{
    //m_ocassionalBBox->Render(shaders["VertexColor"], m_duck->GetModelMatrix(), GetSceneCamera());
    if (!m_inGame)
    {
        m_menu->RenderMenu(shaders["VertexColor"], GetSceneCamera());
    }
    else
    {
        m_shotAnimationTime += deltaTimeSeconds * m_shotAnimationActive;
        if (m_shotAnimationActive == 1)
        {
            m_backGroundColor = Colors::getColorPlusAlpha(Colors::WHITE, 1.f);
            if (m_shotAnimationTime > m_shotAnimationDuration)
            {
                m_shotAnimationTime = 0;
                m_shotAnimationActive = 0;
                m_backGroundColor = Colors::getColorPlusAlpha(Colors::BLACK, 1.f);
            }
        }

        m_duck->Update(deltaTimeSeconds);

        if (m_duck->IsDead())
        {
            m_backGroundColor = Colors::getColorPlusAlpha(Colors::DARK_RED, 1.f);
        }
        else if (m_duck->IsFree())
        {
            m_backGroundColor = Colors::getColorPlusAlpha(Colors::CYAN, 1.f);
        }
        else
        {
            m_duck->CollisionDetectAndAct();
            m_handler->SetProgress(m_duck->GetTimeBeingASlave());
        }

        if (m_duck->IsFree() || m_duck->IsDead())
        {
            m_timeBetweenDucks += deltaTimeSeconds;
            if (m_timeBetweenDucks > m_maxTimeBetweenDucks && m_duck->GetBoundingBox().IsOutsideOfViewPort(m_viewPortSpace))
            {
                if (m_duck->IsFree())
                {
                    m_hearts->LoseAHeart();
                }
                m_backGroundColor = Colors::getColorPlusAlpha(Colors::BLACK, 1.f);
                m_duck.reset(new Duck(m_logicSpace, m_viewPortSpace));
                m_bullets->ResetBullets();
            }
        }

        RenderGameSceneAndComponents(deltaTimeSeconds);
    }
}


void Game::FrameEnd()
{
    //DrawCoordinateSystem();
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
    if (m_inGame)
    {
        m_crosshair->UpdatePosition(mouseX, m_viewPortSpace.GetUpperY() - mouseY);
    }
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    //std::cout << mouseX << " " << m_viewPortSpace.GetUpperY() - mouseY << std::endl;
    //std::cout << m_duck->GetBoundingBox().GetBottomLeftCorner() << std::endl;
    if (m_inGame)
    {
        if (!m_duck->IsFree())
        {
            m_bullets->ShotBullet();

            m_shotAnimationActive = 1;

            m_duck->GotShot({ mouseX, m_viewPortSpace.GetUpperY() - mouseY });

            if (m_bullets->IsCartridgeEmpty() && !m_duck->IsDead())
            {
                m_duck->SetFree();
                m_duck->SetTimeServed();
                m_hearts->LoseAHeart();
            }
        }
    }
    else
    {
        if (m_menu->CheckForStartClick(mouseX, m_viewPortSpace.GetUpperY() - mouseY, window))
        {
            m_inGame = true;
            m_crosshair->UpdatePosition(mouseX, m_viewPortSpace.GetUpperY() - mouseY);
        }
    }
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}
