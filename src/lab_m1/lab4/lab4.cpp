#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>
#include <numbers>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    m_cube1MovingDirection = glm::vec3{ 0,0,0 };
    m_cube1Speed = 5.;

    m_cube2ScaleSpeed = 0.2;
    m_cube2ScaleingDirection = 0;

    m_cube3RotationDirection = glm::vec3{ 0,0,0 };
    m_cube3RotationSpeed = std::numbers::pi / 3;

    m_viewPortMovingDirection = glm::vec2{ 0,0 };
    m_viewPortSpeed = 50;
    m_viewPortScalingSpeed = 50;
    m_viewPortScalingDirection = 0;

    m_1angle = 0.f;
    m_1orbitSize = 2.f;
    m_1rotationSpeed = std::numbers::pi / 5;

    m_2angle = 0.f;
    m_2orbitSize = 0.6f;
    m_2rotationSpeed = std::numbers::pi / 2;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    m_viewPortX = (float)miniViewportArea.x;
    m_viewPortY = (float)miniViewportArea.y;
    m_viewPortHeight = (float)miniViewportArea.height;
    m_viewPortWidth = (float)miniViewportArea.width;
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab4::RenderScene() {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);


    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::RotateOZ(m_1angle);
    modelMatrix *= transform3D::Translate(m_1orbitSize, 0, 0);

    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);


    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::RotateOZ(m_1angle);
    modelMatrix *= transform3D::Translate(m_1orbitSize, 0, 0);
    modelMatrix *= transform3D::RotateOZ(-m_1angle);

    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    modelMatrix *= transform3D::RotateOX(m_2angle);
    modelMatrix *= transform3D::Translate(0, 0, -m_2orbitSize);

    modelMatrix *= transform3D::Scale(0.2, 0.2, 0.2);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void ClampAngle(float& angle)
{
    if (angle > std::numbers::pi * 2)
        angle = 0;
    if (angle < 0)
        angle = std::numbers::pi * 2;
}

void Lab4::Update(float deltaTimeSeconds)
{
    translateX += m_cube1MovingDirection[0] * deltaTimeSeconds * m_cube1Speed;
    translateY += m_cube1MovingDirection[1] * deltaTimeSeconds * m_cube1Speed;
    translateZ += m_cube1MovingDirection[2] * deltaTimeSeconds * m_cube1Speed;

    m_1angle += m_1rotationSpeed * deltaTimeSeconds;
    ClampAngle(m_1angle);

    m_2angle += m_2rotationSpeed * deltaTimeSeconds;
    ClampAngle(m_2angle);

    //float scalingStep = m_cube2ScaleingDirection * deltaTimeSeconds * m_cube2ScaleSpeed;
    //scaleX += scalingStep;
    //scaleY += scalingStep;
    //scaleZ += scalingStep;

    //ClampAngle(angularStepOX);
    //ClampAngle(angularStepOY);
    //ClampAngle(angularStepOZ);
    //angularStepOX -= m_cube3RotationDirection[0] * deltaTimeSeconds * m_cube3RotationSpeed;
    //angularStepOY -= m_cube3RotationDirection[1] * deltaTimeSeconds * m_cube3RotationSpeed;
    //angularStepOZ -= m_cube3RotationDirection[2] * deltaTimeSeconds * m_cube3RotationSpeed;

    //m_viewPortX += m_viewPortMovingDirection[0] * deltaTimeSeconds * m_viewPortSpeed;
    //m_viewPortY += m_viewPortMovingDirection[1] * deltaTimeSeconds * m_viewPortSpeed;

    //m_viewPortWidth += m_viewPortScalingSpeed * deltaTimeSeconds * m_viewPortScalingDirection;
    //m_viewPortHeight += m_viewPortScalingSpeed * deltaTimeSeconds * m_viewPortScalingDirection;

    //int initWidth = miniViewportArea.width;
    //int initHeight = miniViewportArea.height;
    //m_viewPortX -= ((float)std::round(m_viewPortWidth) - initWidth) / 2.;
    //m_viewPortY -= ((float)std::round(m_viewPortHeight) - initHeight) / 2.;

    //miniViewportArea.x = int(std::round(m_viewPortX));
    //miniViewportArea.y = int(std::round(m_viewPortY));
    //miniViewportArea.width = int(std::round(m_viewPortWidth));
    //miniViewportArea.height = int(std::round(m_viewPortHeight));

    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    RenderScene();
    DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    RenderScene();
    // TODO(student): render the scene again, in the new viewport
    DrawCoordinateSystem();
}

void Lab4::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{

}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
    
    switch (key)
    {
    case GLFW_KEY_I:
        m_cube1MovingDirection += glm::vec3{0, 0, -1};
        break;
    case GLFW_KEY_K:
        m_cube1MovingDirection += glm::vec3{ 0, 0, 1 };
        break;
    case GLFW_KEY_J:
        m_cube1MovingDirection += glm::vec3{ -1, 0, 0 };
        break;
    case GLFW_KEY_L:
        m_cube1MovingDirection += glm::vec3{ 1, 0, 0 };
        break;
    case GLFW_KEY_R:
        m_cube1MovingDirection += glm::vec3{ 0, 1, 0 };
        break;
    case GLFW_KEY_F:
        m_cube1MovingDirection += glm::vec3{ 0, -1, 0 };
        break;
    }
    

    switch (key)
    {
    case GLFW_KEY_1:
        m_cube2ScaleingDirection -= 1;
        break;
    case GLFW_KEY_2:
        m_cube2ScaleingDirection += 1;
        break;
    }

    switch (key)
    {
    case GLFW_KEY_3:
        m_cube3RotationDirection += glm::vec3{ 0, 0, -1 };
        break;
    case GLFW_KEY_4:
        m_cube3RotationDirection += glm::vec3{ 0, 0, 1 };
        break;
    case GLFW_KEY_5:
        m_cube3RotationDirection += glm::vec3{ -1, 0, 0 };
        break;
    case GLFW_KEY_6:
        m_cube3RotationDirection += glm::vec3{ 1, 0, 0 };
        break;
    case GLFW_KEY_7:
        m_cube3RotationDirection += glm::vec3{ 0, 1, 0 };
        break;
    case GLFW_KEY_8:
        m_cube3RotationDirection += glm::vec3{ 0, -1, 0 };
        break;
    }

    switch (key)
    {
    case GLFW_KEY_I:
        m_viewPortMovingDirection += glm::vec2{ 0, 1 };
        break;
    case GLFW_KEY_K:
        m_viewPortMovingDirection += glm::vec2{ 0, -1 };
        break;
    case GLFW_KEY_L:
        m_viewPortMovingDirection += glm::vec2{ 1, 0 };
        break;
    case GLFW_KEY_J:
        m_viewPortMovingDirection += glm::vec2{ -1, 0 };
        break;
    }

    switch (key)
    {
    case GLFW_KEY_U:
        m_viewPortScalingDirection += -1;
        break;
    case GLFW_KEY_O:
        m_viewPortScalingDirection += 1;
        break;
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    switch (key)
    {
    case GLFW_KEY_I:
        m_cube1MovingDirection -= glm::vec3{ 0, 0, -1 };
        break;
    case GLFW_KEY_K:
        m_cube1MovingDirection -= glm::vec3{ 0, 0, 1 };
        break;
    case GLFW_KEY_J:
        m_cube1MovingDirection -= glm::vec3{ -1, 0, 0 };
        break;
    case GLFW_KEY_L:
        m_cube1MovingDirection -= glm::vec3{ 1, 0, 0 };
        break;
    case GLFW_KEY_R:
        m_cube1MovingDirection -= glm::vec3{ 0, 1, 0 };
        break;
    case GLFW_KEY_F:
        m_cube1MovingDirection -= glm::vec3{ 0, -1, 0 };
        break;
    }

    switch (key)
    {
    case GLFW_KEY_1:
        m_cube2ScaleingDirection += 1;
        break;
    case GLFW_KEY_2:
        m_cube2ScaleingDirection -= 1;
        break;
    }

    switch (key)
    {
    case GLFW_KEY_3:
        m_cube3RotationDirection -= glm::vec3{ 0, 0, -1 };
        break;
    case GLFW_KEY_4:
        m_cube3RotationDirection -= glm::vec3{ 0, 0, 1 };
        break;
    case GLFW_KEY_5:
        m_cube3RotationDirection -= glm::vec3{ -1, 0, 0 };
        break;
    case GLFW_KEY_6:
        m_cube3RotationDirection -= glm::vec3{ 1, 0, 0 };
        break;
    case GLFW_KEY_7:
        m_cube3RotationDirection -= glm::vec3{ 0, 1, 0 };
        break;
    case GLFW_KEY_8:
        m_cube3RotationDirection -= glm::vec3{ 0, -1, 0 };
        break;
    }

    switch (key)
    {
    case GLFW_KEY_I:
        m_viewPortMovingDirection -= glm::vec2{ 0, 1 };
        break;
    case GLFW_KEY_K:
        m_viewPortMovingDirection -= glm::vec2{ 0, -1 };
        break;
    case GLFW_KEY_L:
        m_viewPortMovingDirection -= glm::vec2{ 1, 0 };
        break;
    case GLFW_KEY_J:
        m_viewPortMovingDirection -= glm::vec2{ -1, 0 };
        break;
    }

    switch (key)
    {
    case GLFW_KEY_U:
        m_viewPortScalingDirection -= -1;
        break;
    case GLFW_KEY_O:
        m_viewPortScalingDirection -= 1;
        break;
    }
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
