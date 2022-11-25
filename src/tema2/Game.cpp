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


	{
		Shader* shader = new Shader("MyShader");
		shader->AddShader(
			PATH_JOIN(window->props.selfDir, SOURCE_PATH::TEMA2, "shaders", "VertexShader.glsl"),
			GL_VERTEX_SHADER);
		shader->AddShader(
			PATH_JOIN(window->props.selfDir, SOURCE_PATH::TEMA2, "shaders", "FragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

    {
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(
			PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "mele"), "F1.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	m_car = std::make_unique<Masina>(window, shaders["VertexNormal"]);

	m_camera = m_car->GetCamera();

	m_components.emplace_back(std::make_unique<Pista>(shaders["MyShader"], m_camera, 4 / 2));

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
	//m_pista->Render();
	//RenderMesh(meshes["box"], shaders["VertexNormal"], glm::mat4{1});
	m_car->Render();

	this->RenderGameComponents();
}

void Game::FrameEnd()
{
	DrawCoordinateSystem(m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
}


void Game::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 10.0f;

		if (window->KeyHold(GLFW_KEY_W))
		{
			// TODO(student): Translate the camera forward
			m_camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A))
		{
			// TODO(student): Translate the camera to the left
			m_camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S))
		{
			// TODO(student): Translate the camera backward
			m_camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D))
		{
			// TODO(student): Translate the camera to the right
			m_camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q))
		{
			// TODO(student): Translate the camera downward
			m_camera->TranslateUpward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E))
		{
			// TODO(student): Translate the camera upward
			m_camera->TranslateUpward(-deltaTime * cameraSpeed);
		}
	}

	//if (window->KeyHold(GLFW_KEY_W))
	//{
	//	engine.Accelerating();
	//}
	//else if (window->KeyHold(GLFW_KEY_S))
	//{
	//	engine.Brake();
	//}
	//else
	//{
	//	engine.InertialDecceleration();
	//}
	//std::cout << engine.GetCurrentGear() << " " << engine.GetSpeed() << std::endl;
	//m_camera->GetPosition() = engine.GetNewPosition(m_camera->GetPosition(), glm::normalize(m_camera->forward), deltaTime);
}


void Game::OnKeyPress(int key, int mods)
{
}


void Game::OnKeyRelease(int key, int mods)
{
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0)
		{
			m_camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
			m_camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
		}
	}
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Game::RenderGameComponents()
{
	std::for_each(
		m_components.begin(), m_components.end(), [](const auto& curr) { curr->Render(); });
}
