#include "Pista.hpp"

#include "tema2/Geometries/PlanarTriangle.hpp"

Pista::Pista(
	Shader* shader,
	const gfxc::Camera* const camera,
	const double width,
	const glm::vec3& origin,
	const double scale)
	: GameComponent(shader, camera, origin, scale), m_width(width)
{
	this->InitTrack();
}

void Pista::InitTrack()
{
	m_trackPoints.reserve(7);
	m_trackPoints.emplace_back(0, -7.8, 0);
	m_trackPoints.emplace_back(19.7, -3.6, 0);
	m_trackPoints.emplace_back(25.7, 7.9, 0);
	m_trackPoints.emplace_back(6.11, 19.518, 0);
	m_trackPoints.emplace_back(-18.47, 13.711, 0);
	m_trackPoints.emplace_back(-22.3, -2.169, 0);

	this->IncreasePointDensityByLinearInterpolation();
	this->IncreasePointDensityByPolynomialInterpolation();

	std::vector<glm::vec3> exteriorPoints = this->GenerateExteriorPoints();
	this->GenerateGeometries(exteriorPoints);
}

void Pista::IncreasePointDensityByLinearInterpolation()
{
	std::vector<glm::vec3> interpolationPoints;

	for (size_t it = 0; it < m_trackPoints.size() - 1; it++)
	{
		double distance = glm::distance(m_trackPoints[it], m_trackPoints[it + 1]);
		if (distance < 2 * m_interpolationStep)
			continue;

		interpolationPoints.push_back(m_trackPoints[it]);
		
		int interpolationSteps = (int)std::floor(distance / m_interpolationStep);
		
		float stepInX = std::abs(m_trackPoints[it].x - m_trackPoints[it + 1].x) / interpolationSteps;
		float stepInY = std::abs(m_trackPoints[it].y - m_trackPoints[it + 1].y) / interpolationSteps;

		for (size_t stepNr = 1; stepNr < interpolationSteps; stepNr++)
		{ 
			interpolationPoints.emplace_back(
				std::min(m_trackPoints[it].x, m_trackPoints[it + 1].x) + stepNr * stepInX,
				std::min(m_trackPoints[it].y, m_trackPoints[it + 1].y) + stepNr * stepInY,
				0);
		}

		interpolationPoints.push_back(m_trackPoints[it + 1]);
	}

	m_trackPoints.clear();
	m_trackPoints = interpolationPoints;
}


void Pista::IncreasePointDensityByPolynomialInterpolation()
{
	return;
}

std::vector<glm::vec3> Pista::GenerateExteriorPoints()
{
	std::vector<glm::vec3> exteriorPoints;
	exteriorPoints.reserve(m_trackPoints.size());

	{
		const glm::vec3 segmentDirection1 =
			*m_trackPoints.cbegin() - *std::prev(m_trackPoints.cend());
		const glm::vec3 exteriorDirection1 = glm::cross(segmentDirection1, m_zAxisNormal);

		const glm::vec3 segmentDirection2 =
			*m_trackPoints.cbegin() - *std::next(m_trackPoints.cbegin());
		const glm::vec3 exteriorDirection2 = glm::cross(m_zAxisNormal, segmentDirection2);

		const glm::vec3 finalExteriorDirection =
			glm::normalize(exteriorDirection1 + exteriorDirection2);

		const glm::vec3 exteriorPoint =
			*m_trackPoints.cbegin() + finalExteriorDirection * static_cast<float>(m_width);

		exteriorPoints.push_back(exteriorPoint);
	}

	for (auto it = m_trackPoints.cbegin(); it != std::prev(m_trackPoints.cend(), 2); ++it)
	{
		const glm::vec3 segmentDirection1 = *std::next(it) - *it;
		const glm::vec3 exteriorDirection1 = glm::cross(segmentDirection1, m_zAxisNormal);

		const glm::vec3 segmentDirection2 = *std::next(it) - *std::next(it, 2);
		const glm::vec3 exteriorDirection2 = glm::cross(m_zAxisNormal, segmentDirection2);

		glm::dot(exteriorDirection1, exteriorDirection2);

		const glm::vec3 finalExteriorDirection =
			glm::normalize(exteriorDirection1 + exteriorDirection2);

		const glm::vec3 exteriorPoint =
			*std::next(it) + finalExteriorDirection * static_cast<float>(m_width);

		exteriorPoints.push_back(exteriorPoint);
	}

	{
		const glm::vec3 segmentDirection1 =
			*std::prev(m_trackPoints.end()) - *std::prev(m_trackPoints.end(), 2);
		const glm::vec3 exteriorDirection1 = glm::cross(segmentDirection1, m_zAxisNormal);

		const glm::vec3 segmentDirection2 =
			*std::prev(m_trackPoints.end()) - *m_trackPoints.begin();
		const glm::vec3 exteriorDirection2 = glm::cross(m_zAxisNormal, segmentDirection2);

		const glm::vec3 finalExteriorDirection =
			glm::normalize(exteriorDirection1 + exteriorDirection2);

		const glm::vec3 exteriorPoint =
			*std::prev(m_trackPoints.end()) + finalExteriorDirection * static_cast<float>(m_width);

		exteriorPoints.push_back(exteriorPoint);
	}

	return exteriorPoints;
}

void Pista::GenerateGeometries(const std::vector<glm::vec3>& exteriorPoints)
{
	m_geometries.reserve(m_trackPoints.size() * 2);

	for (size_t it = 0; it < m_trackPoints.size() - 1; it++)
	{
		m_geometries.emplace_back(new PlanarTriangle(
			m_shader,
			m_camera,
			exteriorPoints[it],
			m_trackPoints[it],
			m_trackPoints[it + 1],
			m_roadColor));
		m_geometries.emplace_back(new PlanarTriangle(
			m_shader,
			m_camera,
			exteriorPoints[it],
			m_trackPoints[it + 1],
			exteriorPoints[it + 1],
			m_roadColor));
	}
	{
		m_geometries.emplace_back(new PlanarTriangle(
			m_shader,
			m_camera,
			*std::prev(exteriorPoints.end()),
			*std::prev(m_trackPoints.end()),
			*m_trackPoints.begin(),
			m_roadColor));
		m_geometries.emplace_back(new PlanarTriangle(
			m_shader,
			m_camera,
			*std::prev(exteriorPoints.end()),
			*m_trackPoints.begin(),
			*exteriorPoints.begin(),
			m_roadColor));
	}
}
