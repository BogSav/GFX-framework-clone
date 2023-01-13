#pragma once

#include "GameComponent.hpp"
#include "Field.hpp"

class Rock : public GameComponent
{
public:
	Rock(
		const Shader* const shader,
		CustomCamera* const camera,
		const WindowObject* window,
		std::shared_ptr<Texture2D>& texture);

	void Render() override;

private:
	Rock(
		const Shader* const shader,
		CustomCamera* const camera,
		std::shared_ptr<Texture2D>& texture,
		std::string meshPath,
		std::string meshName);

private:
	std::shared_ptr<Texture2D> m_texture;
	std::vector<std::unique_ptr<GeometryObject3d>> m_objects;

	const float m_distanceBetweenRocks;
};