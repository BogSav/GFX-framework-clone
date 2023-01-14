#pragma once

#include "GameComponent.hpp"

class Field;

class Player : public GameComponent
{
public:
	Player() = delete;
	Player(const Shader* const shader, CustomCamera* const camera, const Field* field);

	void Render() override;

	void IncreaseCollectedGifts() { m_collectedGifts++; };
	int GetGiftsNr() { return m_collectedGifts; }

	void Reset();

private:
	std::unique_ptr<GeometryObject3d> m_cube;
	std::unique_ptr<GeometryObject3d> m_rightSki;
	std::unique_ptr<GeometryObject3d> m_leftSki;

	int m_collectedGifts = 0;

	const Field* m_field;
};