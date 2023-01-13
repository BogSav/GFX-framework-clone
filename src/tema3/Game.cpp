#include "tema3/Game.hpp"

#include <iostream>

Game::Game() : m_treeNumber(6), m_presentsNumber(6), m_rocksNumber(6)
{
}


Game::~Game()
{
}

void Game::Init()
{
	CreateShaders();
	CreateTextures();

	m_camera = std::make_shared<CustomCamera>(
		glm::vec3{0, 10.15f, 14.15f},
		glm::vec3{0, -5.f, -1.f},
		glm::vec3{0, 0.7f, -0.7f},
		window->props.aspectRatio);

	m_directionalLight =
		std::make_unique<DirectionalLight>(Colors::White, glm::vec3{0, -1, 0}, 0.7f);

	m_field = std::make_unique<Field>(shaders["TexTest"], m_camera, m_textures["Snow"]);

	m_player = std::make_unique<Player>(
		shaders["TexTest"], m_camera.get(), m_field.get());

	m_field->Render();

	m_lightSources.push_back(m_directionalLight.get());

	for (size_t i = 0; i < m_treeNumber; i++)
	{
		m_trees.emplace_back(new Tree(
			shaders["TexTest"], m_camera.get(), window, m_textures["Crown"], m_textures["Trunk"]));
		m_trees[i]->InstantiateLightSource();
		m_trees[i]->Activate(m_field.get());
		m_lightSources.push_back(m_trees[i]->GetLightSoruce());
		m_gameComponents.push_back(m_trees[i].get());
	}

	for (size_t i = 0; i < m_presentsNumber; i++)
	{
		m_presents.emplace_back(
			new Present(shaders["TexTest"], m_camera.get(), window, m_textures["Present"]));
		m_presents[i]->InstantiateLightSource();
		m_presents[i]->Activate(m_field.get());
		m_lightSources.push_back(m_presents[i]->GetLightSource());
		m_gameComponents.push_back(m_presents[i].get());
	}

	for (size_t i = 0; i < m_rocksNumber; i++)
	{
		m_rocks.emplace_back(
			new Rock(shaders["TexTest"], m_camera.get(), window, m_textures["Rock"]));
		m_rocks[i]->Activate(m_field.get());
		m_gameComponents.push_back(m_rocks[i].get());
	}
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

	Render();
	CollectGifts();
	CheckCollisions();

	if (gameOver)
	{
		std::cout << m_player->GetGiftsNr() << std::endl;
		this->Exit();
	}
}

void Game::FrameEnd()
{
	// DrawCoordinateSystem(m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	DeactivateUselessComponents();
	ActivateUselessComponents();
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
	{
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
}

void Game::CreateTextures()
{
	{
		std::shared_ptr<Texture2D> presentTexture = std::make_shared<Texture2D>();
		presentTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "gift.jpg").c_str());
		m_textures["Present"] = presentTexture;
	}

	{
		std::shared_ptr<Texture2D> snowTexture = std::make_shared<Texture2D>();
		snowTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "snow.jpg").c_str());
		m_textures["Snow"] = snowTexture;
	}

	{
		std::shared_ptr<Texture2D> rockTexture = std::make_shared<Texture2D>();
		rockTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "rock.jpg").c_str());
		m_textures["Rock"] = rockTexture;
	}

	{
		std::shared_ptr<Texture2D> crownTexture = std::make_shared<Texture2D>();
		crownTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "treeCrown.jpg").c_str());
		m_textures["Crown"] = crownTexture;
	}

	{
		std::shared_ptr<Texture2D> trunkTexture = std::make_shared<Texture2D>();
		trunkTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "treeTrunk.jpg").c_str());
		m_textures["Trunk"] = trunkTexture;
	}
}

void Game::Render()
{
	m_field->Render();
	m_player->Render();
	std::for_each(
		m_trees.begin(),
		m_trees.end(),
		[this](const auto& curr)
		{
			if (curr->GetActiveState())
				curr->Render(m_camera->GetPosition(), m_lightSources);
		});
	std::for_each(
		m_presents.begin(),
		m_presents.end(),
		[](const auto& curr)
		{
			if (curr->GetActiveState())
				curr->Render();
		});
	std::for_each(
		m_rocks.begin(),
		m_rocks.end(),
		[this](const auto& curr)
		{
			if (curr->GetActiveState())
				curr->Render();
		});
}

void Game::DeactivateUselessComponents()
{
	std::for_each(
		m_gameComponents.begin(),
		m_gameComponents.end(),
		[this](const auto curr)
		{
			if (!CollisionEngine::AreColliding(m_field->GetBBox(), curr->GetBBox()))
				curr->Deactivate();
		});
}

void Game::ActivateUselessComponents()
{
	std::for_each(
		m_gameComponents.begin(),
		m_gameComponents.end(),
		[this](const auto curr)
		{
			if (!curr->GetActiveState())
			{
				do
				{
					curr->Activate(m_field.get());
					curr->Render();
				} while (CollisionEngine::AreColliding(m_player->GetBBox(), curr->GetBBox()));
			}
		});
}

void Game::CollectGifts()
{
	std::for_each(
		m_presents.begin(),
		m_presents.end(),
		[this](const auto& curr)
		{
			if (curr->GetActiveState()
				&& CollisionEngine::AreColliding(m_player->GetBBox(), curr->GetBBox()))
			{
				m_player->IncreaseCollectedGifts();
				curr->Deactivate();
			}
		});
}

void Game::CheckCollisions()
{
	std::for_each(
		m_rocks.begin(),
		m_rocks.end(),
		[this](const auto& curr)
		{
			if (CollisionEngine::AreColliding(m_player->GetBBox(), curr->GetBBox()))
				gameOver = true;
		});
}
