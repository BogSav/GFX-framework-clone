#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
    :
    m_initialPosition(0, 0, 0),
    m_sequentialMovement(true)
{
    // TODO(student): Never forget to initialize class variables!
    m_clearColor = { 0, 0, 0, 1 };
    m_velocity = glm::vec3(0, 0, 0);
    m_totalMovement = m_initialPosition;
    m_speed = 2.f;
    m_currentMeshIdx = 0;
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.
    m_meshNames.reserve(meshes.size());
    std::for_each(meshes.begin(), meshes.end(), [this]
        (const std::pair<std::string, Mesh*>& current)
        {
            m_meshNames.push_back(current.second->GetMeshID());
        });
}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

    if((m_sequentialMovement && m_movementKeyBeingPressed) || !m_sequentialMovement)
        m_totalMovement += (m_velocity * deltaTimeSeconds);

    RenderMesh(meshes[m_meshNames[m_currentMeshIdx]], m_totalMovement);
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.

}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        m_clearColor[0] = 200;
        m_clearColor[1] = 0;
        m_clearColor[2] = 75;
        m_clearColor[3] = 0.2;
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.
    switch (key)
    {
    case GLFW_KEY_W:
        m_velocity = (glm::vec3(0, 0, -1) * m_speed);
        break;
    case GLFW_KEY_A:
        m_velocity = (glm::vec3(-1, 0, 0) * m_speed);
        break;
    case GLFW_KEY_S:
        m_velocity = (glm::vec3(0, 0, 1) * m_speed);
        break;
    case GLFW_KEY_D:
        m_velocity = (glm::vec3(1, 0, 0) * m_speed);
        break;
    case GLFW_KEY_Q:
        m_velocity = (glm::vec3(0, 1, 0) * m_speed);
        break;
    case GLFW_KEY_E:
        m_velocity = (glm::vec3(0, -1, 0) * m_speed);
        break;
    }

    switch (key)
    {
    case GLFW_KEY_W:
    case GLFW_KEY_A:
    case GLFW_KEY_S:
    case GLFW_KEY_D:
    case GLFW_KEY_Q:
    case GLFW_KEY_E:
        m_movementKeyBeingPressed = true;
        break;
    }

    switch (key)
    {
    case GLFW_KEY_J:
        m_currentMeshIdx--;
        if (m_currentMeshIdx == -1)
            m_currentMeshIdx = m_meshNames.size() - 1;
        break;
    case GLFW_KEY_K:
        m_currentMeshIdx++;
        if (m_currentMeshIdx == m_meshNames.size())
            m_currentMeshIdx = 0;
        break;
    }
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
    m_movementKeyBeingPressed = false;
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
