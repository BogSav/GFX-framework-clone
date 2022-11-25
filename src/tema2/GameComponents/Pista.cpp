#include "Pista.hpp"

#include "tema2/Geometries/PlanarTriangle.hpp"

#include <fstream>

Pista::Pista(
	Shader* shader,
	const CustomCamera* const camera,
	const double width,
	const glm::vec3& origin,
	const double scale)
	: GameComponent(shader, camera, origin, scale), m_width(width)
{
	this->InitTrack();
	m_modelMatrix = glm::scale(m_modelMatrix, {6, 0, 6});
}

void Pista::InitTrack()
{
	this->InitTrackPointsFromFile();

	m_exteriorPoints = this->GenerateExteriorPoints();

	this->GenerateGeometries();
}

void Pista::InitTrackPointsFromFile()
{
	std::string fileName = m_trackName + ".txt";
	std::string path = PATH_JOIN(SOURCE_PATH::TEMA2, "Piste", fileName);
	
	std::ifstream inputf;

	try
	{
		inputf.open(path);
		
		if (inputf.fail())
			throw std::exception("Nu exista");

		size_t size;
		inputf >> size;

		m_trackPoints.reserve(size);
		
		for (size_t i = 0; i < size; i++)
		{
			float x, y, z;
			inputf >> x >> y >> z;

			m_trackPoints.emplace_back(x, 0, -y);
		}

		inputf.close();
	}
	catch (std::exception e)
	{
		throw e;
	}
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
			glm::normalize((exteriorDirection1 + exteriorDirection2) / 2.f);

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

void Pista::GenerateGeometries()
{
	m_geometries.reserve(m_trackPoints.size() * 2);

	for (size_t it = 0; it < m_trackPoints.size() - 1; it++)
	{
		m_geometries.emplace_back(new PlanarTriangle(
			m_shader,
			m_camera,
			m_exteriorPoints[it],
			m_trackPoints[it],
			m_trackPoints[it + 1],
			m_roadColor));
		m_geometries.emplace_back(new PlanarTriangle(
			m_shader,
			m_camera,
			m_exteriorPoints[it],
			m_trackPoints[it + 1],
			m_exteriorPoints[it + 1],
			m_roadColor));
	}
	{
		m_geometries.emplace_back(new PlanarTriangle(
			m_shader,
			m_camera,
			*std::prev(m_exteriorPoints.end()),
			*std::prev(m_trackPoints.end()),
			*m_trackPoints.begin(),
			m_roadColor));
		m_geometries.emplace_back(new PlanarTriangle(
			m_shader,
			m_camera,
			*std::prev(m_exteriorPoints.end()),
			*m_trackPoints.begin(),
			*m_exteriorPoints.begin(),
			m_roadColor));
	}
}
