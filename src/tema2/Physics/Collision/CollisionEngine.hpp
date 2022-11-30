#pragma once

#include "tema2/GameComponents/GameComponent.hpp"
#include "tema2/GameComponents/Car.hpp"
#include "tema2/GameComponents/Track.hpp"
#include "tema2/GameComponents/NPC.hpp"

class CollisionEngine
{
private:
	CollisionEngine();

public:
	static bool IsOnTrack(const Track* pista, const GameComponent* component);

	template <class T>
	static void IsCollidingWithNPC(const Car* masina, const T* gameObject, bool& IsColliding);

private:
	static inline constexpr bool IsInside(
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& v3,
		const glm::vec3& v4,
		const glm::vec3& point);
};
