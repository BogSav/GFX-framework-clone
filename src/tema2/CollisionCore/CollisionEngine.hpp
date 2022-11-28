#pragma once

#include "tema2/GameComponents/GameComponent.hpp"
#include "tema2/GameComponents/Masina.hpp"
#include "tema2/GameComponents/MasinaObstacol.hpp"
#include "tema2/GameComponents/Pista.hpp"

class CollisionEngine
{
private:
	CollisionEngine();

public:
	static bool IsOnTrack(const Pista* pista, const Masina* masina);
	static bool IsOnTrack(const Pista*, const class Tree*);

	template <class T>
	static constexpr void IsCollidingWithNPC(
		const Masina* masina, T* gameObject, bool& IsColliding);

private:
	static constexpr bool IsInside(
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& v3,
		const glm::vec3& v4,
		const glm::vec3& point);
};
