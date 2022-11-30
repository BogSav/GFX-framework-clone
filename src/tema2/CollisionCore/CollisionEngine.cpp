#include "CollisionEngine.hpp"

#include "tema2/GameComponents/Copac.hpp"

bool CollisionEngine::IsOnTrack(const Pista* pista, const GameComponent* component)
{
	bool isOnTrack = false;
	if (const Masina* masina = dynamic_cast<const Masina*>(component))
	{
		for (size_t it = 0; it < pista->m_trackPoints.size() - 1; it++)
		{
			if (CollisionEngine::IsInside(
					pista->m_exteriorPoints[it],
					pista->m_trackPoints[it],
					pista->m_trackPoints[it + 1],
					pista->m_exteriorPoints[it + 1],
					masina->m_position))
			{
				isOnTrack = true;
				break;
			}
		}
		return isOnTrack;
	}

	if (const Tree* tree = dynamic_cast<const Tree*>(component))
	{
		for (size_t it = 0; it < pista->m_trackPoints.size() - 1; it++)
		{
			if (CollisionEngine::IsInside(
					pista->m_exteriorPoints[it],
					pista->m_trackPoints[it],
					pista->m_trackPoints[it + 1],
					pista->m_exteriorPoints[it + 1],
					tree->GetTrunkCenter()))
			{
				isOnTrack = true;
				break;
			}
		}
		return isOnTrack;
	}

	throw std::exception("Wtf are you trying to check?");
}

template <class T>
void CollisionEngine::IsCollidingWithNPC(
	const Masina* masina, const T* possibleNpc, bool& IsColliding)
{
	if (IsColliding)
		return;

	if (const MasinaObstacol* npc = dynamic_cast<const MasinaObstacol*>(possibleNpc))
	{
		IsColliding = CollisionEngine::IsInside(
			npc->m_position,
			npc->m_position + npc->GetNormalToDirection() * npc->m_width,
			npc->m_position + npc->GetAntiMoveDirection() * npc->m_length
				+ npc->GetNormalToDirection() * npc->m_width,
			npc->m_position + npc->GetAntiMoveDirection() * npc->m_length,
			masina->m_position);
	}
	else
	{
		IsColliding = false;
	}
}

constexpr inline bool CollisionEngine::IsInside(
	const glm::vec3& v1,
	const glm::vec3& v2,32
	const glm::vec3& v3,
	const glm::vec3& v4,
	const glm::vec3& point)
{
	return (
		glm::cross(point - v1, v2 - v1).y >= 0 && glm::cross(point - v2, v3 - v2).y >= 0
		&& glm::cross(point - v3, v4 - v3).y >= 0 && glm::cross(point - v4, v1 - v4).y >= 0);
}
