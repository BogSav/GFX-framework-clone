#pragma once

#include "GameComponent.hpp"

class Pista : public GameComponent
{
public:
	Pista() = delete;
	Pista(
		Shader* shader,
		const CustomCamera* const camera,
		const double width);

	const std::vector<glm::vec3>& GetInteriorPoints() const { return m_trackPoints; };

	friend class CollisionEngine;

private:
	void InitTrack();
	void InitTrackPointsFromFile();

	std::vector<glm::vec3> GenerateExteriorPoints();
	void GenerateGeometries();
	void GenerateLines();

private:
	std::vector<glm::vec3> m_trackPoints;
	std::vector<glm::vec3> m_exteriorPoints;

	Color m_roadColor = Colors::Gray;
	Color m_linesColor = Colors::White;

	const double m_width;
	const glm::vec3 m_zAxisNormal = glm::vec3{0, 1, 0};

	const std::string m_trackName = "MonacoF1";
};