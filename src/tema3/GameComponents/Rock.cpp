#include "Rock.hpp"

Rock::Rock(
	const Shader* const shader,
	CustomCamera* const camera,
	std::shared_ptr<Texture2D>& texture,
	std::string meshPath,
	std::string meshName)
	: m_distanceBetweenRocks(0.6f), GameComponent()
{
	m_texture = texture;

	m_scale = 1.4f;

	m_objects.emplace_back(new GeometryObject3d(shader, camera, meshPath, meshName));
	m_objects.emplace_back(new GeometryObject3d(shader, camera, meshPath, meshName));
	m_objects.emplace_back(new GeometryObject3d(shader, camera, meshPath, meshName));

	std::for_each(
		m_objects.begin(),
		m_objects.end(),
		[](const std::unique_ptr<GeometryObject3d>& curr) { curr->ComputeInitialBBox(); });
}

Rock::Rock(
	const Shader* const shader,
	CustomCamera* const camera,
	const WindowObject* window,
	std::shared_ptr<Texture2D>& texture)
	: Rock(
		shader,
		camera,
		texture,
		PATH_JOIN(window->props.selfDir, "assets", "models", "primitives"),
		"sphere.obj")
{
}

void Rock::Render()
{
	m_modelMatrix = glm::mat4(1);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{m_scale, m_scale, m_scale});

	m_objects[0]->Render(m_modelMatrix, m_texture.get());
	m_bbox = m_objects[0]->GetTranformedBBox(m_modelMatrix);

	std::for_each(
		std::next(m_objects.begin()),
		m_objects.end(),
		[this](const std::unique_ptr<GeometryObject3d>& curr)
		{
			m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3{m_distanceBetweenRocks, 0, 0});
			m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{1.1, 1.1, 1.1});

			curr->Render(m_modelMatrix, m_texture.get());
			m_bbox += curr->GetTranformedBBox(m_modelMatrix);
		});
}
