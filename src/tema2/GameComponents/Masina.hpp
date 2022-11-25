#pragma once

#include "tema2/Utilities/Camera.hpp"
#include "tema2/Physics/Engine.hpp"

#include "components/simple_scene.h"
#include "core/engine.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


class Masina
{
public:
	Masina(const WindowObject*, Shader*);

	void Render();
	void Render(const CustomCamera* const camera);
	

	CustomCamera* GetCamera() { return m_camera; };

private:
	Shader* m_shader;
	CustomCamera* m_camera;

	physics::Engine* m_engine;
	GearBox* m_gearBox;

	std::unique_ptr<Mesh> m_mesh;


	glm::mat4 m_modelMatrix;
};