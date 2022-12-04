#include "tema2/Game.hpp"

#include "tema2/GameComponents/Lighting/StreetLight.hpp"
#include "tema2/GameComponents/NPC.hpp"
#include "tema2/GameComponents/Tree.hpp"
#include "tema2/Physics/Collision/CollisionEngine.hpp"
#include "tema2/Utilities/Utilities.hpp"

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
			PATH_JOIN(
				window->props.selfDir, SOURCE_PATH::TEMA2, "Shaders", "SimpleVertexShader.glsl"),
			GL_VERTEX_SHADER);
		shader->AddShader(
			PATH_JOIN(window->props.selfDir, SOURCE_PATH::TEMA2, "Shaders", "FragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("Lighting");
		shader->AddShader(
			PATH_JOIN(
				window->props.selfDir, SOURCE_PATH::TEMA2, "Shaders", "LightVertexShader.glsl"),
			GL_VERTEX_SHADER);
		shader->AddShader(
			PATH_JOIN(window->props.selfDir, SOURCE_PATH::TEMA2, "Shaders", "FragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	m_car = std::make_unique<Car>(window, shaders["VertexNormal"]);

	m_camera = m_car->GetCamera();
	// m_camera->Set(
	// glm::vec3(0, 2, 3.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), window->props.aspectRatio);

	m_components.emplace_back(new Track(shaders["Lighting"], m_camera.get(), 20));
	m_components.emplace_back(
		new Field(shaders["Lighting"], m_camera.get(), glm::vec3{-450, 0, -300}, 850, 600));

	// if (const Track* pista = dynamic_cast<const Track*>(m_components[0].get()))
	//{
	//	for (size_t i = 0; i < 3; i++)
	//		m_components.emplace_back(NPC::CreateNewNPCRandomized(
	//			pista->GetInteriorPoints(), shaders["Lighting"], m_camera.get()));
	// }

	if (const Track* pista = dynamic_cast<const Track*>(m_components[0].get()))
	{
		if (const Field* field = dynamic_cast<const Field*>(m_components[1].get()))
		{
			for (size_t i = 0; i < 10; i++)
				m_components.emplace_back(
					Tree::GenerateRandomTree(shaders["Lighting"], m_camera.get(), pista, field));
		}
	}

	if (const Track* pista = dynamic_cast<const Track*>(m_components[0].get()))
	{
		if (const Field* field = dynamic_cast<const Field*>(m_components[1].get()))
		{
			for (size_t i = 0; i < 10; i++)
			{
				StreetLight* strLight = StreetLight::GenerateRandomStreetLight(
					shaders["MyShader"], m_camera.get(), pista, field);
				m_lightingComponents.push_back(strLight);
				m_components.emplace_back(strLight);
			}
		}
	}


	m_minimap = std::make_unique<MiniMap>(window, glm::vec2{900, 50}, 300.f, 150.f, -30.f, -30.f);

	m_screen = std::make_unique<ScreenElements>(window, m_minimap.get(), m_car->GetTurometru());
}


void Game::FrameStart()
{
	//glClearColor(0.48f, 0.6f, 0.63f, 1.f);
	glClearColor(0.16, 0.16, 0.16, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::Update(float deltaTimeSeconds)
{
	m_car->Update(deltaTimeSeconds);

	// m_car->PrintData();

	// std::cout << std::boolalpha
	//	  << CollisionEngine::IsOnTrack(
	//			 dynamic_cast<const Track*>(m_components[0].get()), m_car.get())
	//	  << std::endl;

	// bool isInCollision = false;
	// std::for_each(
	//	m_components.begin(),
	//	m_components.end(),
	//	[this, &isInCollision](const auto& curr)
	//	{ CollisionEngine::IsCollidingWithNPC(m_car.get(), curr.get(), isInCollision); });
	// std::cout << std::boolalpha << isInCollision << std::endl;
	// if (isInCollision)
	//	m_car->RestoreLastState();

	 if (frametimer.PassedTime(0.5))
	 std::cout << 1 / deltaTimeSeconds << std::endl;

	m_minimap->UpdateMinimapCameraBasedOnCarPosition(m_car.get());

	this->UpdateGameComponents(deltaTimeSeconds);

	this->RenderGameComponents();
	this->RenderScreenObjects();
	this->RenderMinimap();
}

void Game::FrameEnd()
{
	// DrawCoordinateSystem(m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
}


void Game::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 50.f;

		if (window->KeyHold(GLFW_KEY_W))
		{
			m_camera->TranslateForward(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_A))
		{
			m_camera->TranslateRight(-deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_S))
		{
			m_camera->TranslateForward(-deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_D))
		{
			m_camera->TranslateRight(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_Q))
		{
			m_camera->TranslateUpward(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_E))
		{
			m_camera->TranslateUpward(-deltaTime * cameraSpeed);
		}
	}
	else
	{
		if (window->KeyHold(GLFW_KEY_W))
		{
			m_car->Accelerate();
		}
		else if (window->KeyHold(GLFW_KEY_S))
		{
			m_car->Brake();
		}
		else
		{
			m_car->InertialDecceleration();
		}

		if (window->KeyHold(GLFW_KEY_A))
		{
			m_car->UpdateOrientation(deltaTime);
		}
		else if (window->KeyHold(GLFW_KEY_D))
		{
			m_car->UpdateOrientation(-deltaTime);
		}
	}
}


void Game::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE)
	{
		std::cout << "sdafasdf";
		m_car->RestoreLastState();
	}
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
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);

	std::for_each(
		m_components.begin(),
		m_components.end(),
		[this](const auto& curr)
		{ curr->Render(m_car->GetPosition(), m_camera->GetPosition(), 0, m_lightingComponents); });
	// Fcatorul bun este cam 0.001 keep in mind
	
	m_car->Render();
}

void Game::RenderMinimap()
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(
		static_cast<int>(m_minimap->GetX()),
		static_cast<int>(m_minimap->GetY()),
		static_cast<int>(m_minimap->GetWidth()),
		static_cast<int>(m_minimap->GetHeight()));

	std::for_each(
		m_components.begin(),
		m_components.end(),
		[this](const auto& curr) { curr->Render(shaders["MyShader"], m_minimap->GetCamera()); });

	// m_car->Render(m_minimap->GetCamera(), shaders["VertexNormal"]);
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
	m_screen->RenderCarRepresentation();
}

void Game::RenderScreenObjects()
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);

	m_screen->Render();
}

void Game::UpdateGameComponents(float deltaTime)
{
	std::for_each(
		m_components.begin(),
		m_components.end(),
		[this, &deltaTime](const auto& curr) { curr->Update(deltaTime); });
}
