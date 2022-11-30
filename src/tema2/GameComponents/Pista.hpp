#pragma once

#include "GameComponent.hpp"

class Pista : public GameComponent
{
public:
	Pista() = delete;
	Pista(
		const Shader* const shader,
		CustomCamera* const camera,
		const double& width,
		Color roadColor = Colors::Asfalt,
		Color linesColor = Colors::White,
		std::string trackName = "MonacoF1");

	const std::vector<glm::vec3>& GetInteriorPoints() const { return m_trackPoints; };

	friend class CollisionEngine;

private:
	void InitTrack();
	void ReadTrackPointsFromFile();

	void GenerateExteriorPoints();
	void GenerateGeometries();
	void GenerateLines();

private:
	std::vector<glm::vec3> m_trackPoints;
	std::vector<glm::vec3> m_exteriorPoints;

	const Color m_roadColor = Colors::Gray;
	const Color m_linesColor;

	const double m_width;
	const glm::vec3 m_zAxisNormal = glm::vec3{0, 1, 0};
	const std::string m_trackName;
};