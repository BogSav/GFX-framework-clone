#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>
#include <numbers>
#include <math.h>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?
    cx = corner[0] + squareSide / 2;
    cy = corner[1] + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;
    m_growing = true;
    m_scalingSpeed = 1.5;
    m_maxScale = 2;
    m_minScale = 0.9;

    // Initialize angularStep
    angularStep = std::numbers::pi / 2;
    m_totalAngle = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void clampAngle(float& angle)
{
    angle = std::fmod(angle,2. * std::numbers::pi_v<float>);
    if(angle > std::numbers::pi_v<float>)
        angle -= (2. * std::numbers::pi_v<float>);
}

void Lab3::Update(float deltaTimeSeconds)
{
    m_totalAngle += angularStep * deltaTimeSeconds;
    clampAngle(m_totalAngle);

    if (scaleX < m_maxScale && m_growing)
    {
        m_growing = true;
        scaleX += m_scalingSpeed * deltaTimeSeconds;
        scaleY += m_scalingSpeed * deltaTimeSeconds;

    }  
    else
    {
        m_growing = false;
        scaleX -= m_scalingSpeed * deltaTimeSeconds;
        scaleY -= m_scalingSpeed * deltaTimeSeconds;
        if (scaleX < m_minScale)
            m_growing = true;
    }

    if (translateX < 700 && !m_firstGoingBack)
    {
        translateX += deltaTimeSeconds * 200;
    }
    else
    {
        m_firstGoingBack = true;
        translateX -= deltaTimeSeconds * 100;
        if (translateX < 0)
            m_firstGoingBack = false;
    }

    if (translateY < 300 && !m_firstGoingBack)
    {
        translateY += deltaTimeSeconds * 90;
    }
    else
    {
        m_firstGoingBack = true;
        translateY -= deltaTimeSeconds * 80;
        if (translateX < 0)
            m_firstGoingBack = false;
    }

    // Square 1
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(150 + cx, 250 + cy);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    modelMatrix *= transform2D::Rotate(m_totalAngle);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);


    // Square 2
    modelMatrix = glm::mat3(1);    
    modelMatrix *= transform2D::Translate(400 + translateX, 250);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);

    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    
    // Sqwuare 3
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(650 + cx, 250 + translateY + cy);
    modelMatrix *= transform2D::Rotate(m_totalAngle);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
