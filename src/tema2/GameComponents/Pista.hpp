#pragma once

#include "GameComponent.hpp"

class Pista : public GameComponent
{
public:
	Pista() = delete;
	Pista(
		Shader* shader,
		const CustomCamera* const camera,
		const double width,
		const glm::vec3& origin = glm::vec3{0, 0, 0},
		const double scale = 1.);

private:
	void InitTrack();
	void InitTrackPointsFromFile();


	std::vector<glm::vec3> GenerateExteriorPoints();
	void GenerateGeometries();

private:
	std::vector<glm::vec3> m_trackPoints;
	std::vector<glm::vec3> m_exteriorPoints;

	Color m_roadColor = Colors::LightGray;
	const double m_width;
	const glm::vec3 m_zAxisNormal = glm::vec3{0, 1, 0};

	const double m_interpolationStep = 1;

	const std::string m_trackName = "MonacoF1";
};