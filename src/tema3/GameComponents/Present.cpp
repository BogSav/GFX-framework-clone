#include "Present.hpp"

Present::Present(
	const Shader* const shader,
	CustomCamera* const camera,
	std::shared_ptr<Texture2D>& texture,
	std::string meshPath,
	std::string meshName) : GameComponent()
{
	m_object = std::make_unique<GeometryObject3d>(shader, camera, meshPath, meshName);
	m_object->ComputeInitialBBox();

	m_texture = texture;
}

Present::Present(
	const Shader* const shader,
	CustomCamera* const camera,
	const WindowObject* window,
	std::shared_ptr<Texture2D>& texture)
	: Present(
		shader,
		camera,
		texture,
		PATH_JOIN(window->props.selfDir, "assets", "models", "primitives"),
		"box.obj")
{
}

void Present::Render()
{
	m_modelMatrix = glm::mat4(1);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position + glm::vec3{0, 0.5f, 0});
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{m_scale, m_scale, m_scale});
	m_modelMatrix = glm::rotate(m_modelMatrix, RADIANS(30), glm::vec3{1, 0, 0});

	m_object->Render(m_modelMatrix, m_texture.get());
	m_bbox = m_object->GetTranformedBBox(m_modelMatrix);
}

void Present::InstantiateLightSource()
{
	m_lightSource =
		std::make_unique<PointLight>(m_position + glm::vec3{0, 1, 0}, Colors::MakeRandomRGB(), 1.f);
}

void Present::Activate(const Field* field)
{
	GameComponent::Activate(field);

	m_lightSource->SetPosition(m_position + glm::vec3{0, 1, 0});
}
