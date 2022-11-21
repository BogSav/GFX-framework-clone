#pragma once

#include "GameComponent.hpp"

class Pista : public GameComponent
{
public:
	Pista() = delete;
	Pista(
		Shader* shader,
		const gfxc::Camera* const camera,
		const double width,
		const glm::vec3& origin = glm::vec3{0, 0, 0},
		const double scale = 1.);

private:
	void InitTrack();
	
	void IncreasePointDensityByLinearInterpolation();
	void IncreasePointDensityByPolynomialInterpolation();

	std::vector<glm::vec3> GenerateExteriorPoints();
	void GenerateGeometries(const std::vector<glm::vec3>&);

private:
	std::vector<glm::vec3> m_trackPoints;

	Color m_roadColor = Colors::LightGray;
	const double m_width;
	const glm::vec3 m_zAxisNormal = glm::vec3{0, 0, 1};

	const double m_interpolationStep = 1;
};