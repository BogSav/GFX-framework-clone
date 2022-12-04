#pragma once

#include "tema2/GameComponents/GameComponent.hpp"

class CollisionEngine
{
private:
	CollisionEngine();

public:
	static bool IsOnTrack(const class Track* track, const class Car* car);
	static bool IsOnTrack(const class Track* track, const class StreetLight* streetLight);
	static bool IsOnTrack(const class Track* track, const class Tree* tree);

	template <class T>
	static void IsCollidingWithNPC(const class Car* masina, const T* gameObject, bool& IsColliding);

private:
	static inline constexpr bool IsInside(
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& v3,
		const glm::vec3& v4,
		const glm::vec3& point);

	static inline bool IsOnTrack(const class Track* track, const glm::vec3& pos);
};
