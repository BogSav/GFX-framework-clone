#pragma once

#include "GameComponent.hpp"

class Field;

class Player : public GameComponent
{
public:
	Player() = delete;
	Player(const Shader* const shader, CustomCamera* const camera, const Field* field);

	void Render() override;
	void Update(float) override;

private:
	std::unique_ptr<GeometryObject3d> m_cube;
	std::unique_ptr<GeometryObject3d> m_rightSki;
	std::unique_ptr<GeometryObject3d> m_leftSki;

	const Field* m_field;

	float m_scale;
};