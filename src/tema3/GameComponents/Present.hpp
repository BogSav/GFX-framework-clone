#pragma once

#include "Field.hpp"
#include "GameComponent.hpp"


class Present : public GameComponent
{
public:
	Present(
		const Shader* const shader,
		CustomCamera* const camera,
		const WindowObject* window,
		std::shared_ptr<Texture2D>& texture);

	void Render() override;

	void InstantiateLightSource();
	void Activate(const Field* field) override;

	const LightSourceAdapter* GetLightSource() const { return m_lightSource.get(); }

private:
	Present(
		const Shader* const shader,
		CustomCamera* const camera,
		std::shared_ptr<Texture2D>& texture,
		std::string meshPath,
		std::string meshName);

private:
	std::unique_ptr<GeometryObject3d> m_object;
	std::shared_ptr<Texture2D> m_texture;

	std::unique_ptr<LightSourceAdapter> m_lightSource;
};