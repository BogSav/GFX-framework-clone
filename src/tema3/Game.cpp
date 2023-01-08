#include "tema3/Game.hpp"

Game::Game()
{
}


Game::~Game()
{
}

void Game::Init()
{
	CreateShaders();

	m_camera = std::make_unique<CustomCamera>(
		glm::vec3{0, 11, 11}, glm::vec3{0, 0, -1}, glm::vec3{0, 1, 0}, window->props.aspectRatio);

	const std::string sourceTextureDir =
		PATH_JOIN(window->props.selfDir, "assets", "textures", "snow.jpg");
	m_field = std::make_unique<Field>(shaders["TexTest"], m_camera.get(), sourceTextureDir);

	m_player = std::make_unique<Player>(
		shaders["TexTest"], m_camera.get(), dynamic_cast<const Field*>(m_field.get()));
}


void Game::FrameStart()
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::Update(float deltaTimeSeconds)
{
	m_field->Update(deltaTimeSeconds);
	m_player->Update(deltaTimeSeconds);

	// m_field->Render(shaders["TexTest"], m_camera.get());
	m_field->Render();
	m_player->Render();
}

void Game::FrameEnd()
{
	DrawCoordinateSystem(m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
}


void Game::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		// Control for the camera independent movement
		float cameraSpeed = 5.f;

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
}

void Game::OnKeyPress(int key, int mods)
{
}

void Game::OnKeyRelease(int key, int mods)
{
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	dynamic_cast<Field*>(m_field.get())
		->UpdateTextureDirection(mouseX, mouseY, window->GetResolution());

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

void Game::CreateShaders()
{
	// Simple shader is a shader that doesn't use any lightiong but have suppport for the curvature
	Shader* shader = new Shader("TexTest");
	shader->AddShader(
		PATH_JOIN(window->props.selfDir, SOURCE_PATH::TEMA3, "Shaders", "TexVShader.glsl"),
		GL_VERTEX_SHADER);
	shader->AddShader(
		PATH_JOIN(window->props.selfDir, SOURCE_PATH::TEMA3, "Shaders", "TexFShader.glsl"),
		GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;
}