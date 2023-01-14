#pragma once

#include "components/simple_scene.h"
#include "tema3/Utilities/Camera.hpp"

#include "tema3/GameComponents/Field.hpp"
#include "tema3/GameComponents/Present.hpp"
#include "tema3/GameComponents/Rock.hpp"
#include "tema3/GameComponents/Tree.hpp"
#include "tema3/GameComponents/StreetLight.hpp"

#include "tema3/Utilities/Timer.hpp"
#include "tema3/Text/TextEngine.hpp"

class Game : public gfxc::SimpleScene
{
public:
	Game();
	~Game();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;

	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

	void CreateShaders();
	void CreateTextures();

	void Render();
	void UpdateGameComponents(float);

	void DeactivateUselessComponents();
	void ActivateUselessComponents(); 

	void CollectGifts();
	void CheckCollisions();

private:
	std::shared_ptr<CustomCamera> m_camera;

	std::unique_ptr<Field> m_field;
	std::unique_ptr<Player> m_player;

	std::vector<std::unique_ptr<Tree>> m_trees;
	std::vector<std::unique_ptr<Present>> m_presents;
	std::vector<std::unique_ptr<Rock>> m_rocks;
	std::vector<std::unique_ptr<StreetLight>> m_streetLights;

	std::vector<GameComponent*> m_gameComponents;
	std::vector<const LightSourceAdapter*> m_lightSources;
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textures;

	std::unique_ptr<LightSourceAdapter> m_directionalLight;

	std::unique_ptr<TextEngine> m_textEngine;

	const size_t m_treeNumber;
	const size_t m_presentsNumber;
	const size_t m_rocksNumber;
	const size_t m_pillarsNumber;

	bool gameOver = false;
	bool collisionActive = false;

	DTimer frameTimer;
};