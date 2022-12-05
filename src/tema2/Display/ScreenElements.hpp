#pragma once

#include "2dPolygon.hpp"
#include "Minimap.hpp"
#include "Speedometer.hpp"
#include "tema2/Utilities/Utilities.hpp"

class ScreenElements
{
public:
	ScreenElements() = delete;
	ScreenElements(
		const WindowObject* window,
		const std::shared_ptr<MiniMap>& miniMap,
		const std::shared_ptr<Speedometer>& speedometer)
		: m_NDCSpace(-1.f, -1.f, 2.f, 2.f),
		  m_logicSpace(
			  0,
			  0,
			  static_cast<float>(window->GetResolution().x),
			  static_cast<float>(window->GetResolution().y))
	{
		// Creare matrice de transformare
		m_transformMatrix = utils::VisualizationTransf2d(m_logicSpace, m_NDCSpace);

		m_speedometer = speedometer;
		m_miniMap = miniMap;

		this->CreateShader(window);
		this->CreateMinimapFrame();
	}

	void Render() const
	{
		m_minimapFrame->Render(m_shader.get(), m_transformMatrix, glm::mat3(1));
		m_speedometer->Render(m_shader.get(), m_transformMatrix, glm::mat3(1));
	}

	void RenderCarRepresentation() const
	{
		m_miniMap->RenderCarRepresentation(m_shader.get(), m_transformMatrix, glm::mat3(1));
	}

private:
	void CreateShader(const WindowObject* window)
	{
		// Creare shader 2d
		m_shader = std::make_unique<Shader>("TestShader");
		m_shader->AddShader(
			PATH_JOIN(window->props.selfDir, SOURCE_PATH::TEMA2, "Shaders", "2dVertexShader.glsl"),
			GL_VERTEX_SHADER);
		m_shader->AddShader(
			PATH_JOIN(
				window->props.selfDir, SOURCE_PATH::TEMA2, "Shaders", "2dFragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		m_shader->CreateAndLink();
	}

	void CreateMinimapFrame()
	{
		// Setare propietati frame minimap
		m_minmapFrameColor = Colors::White;
		m_minimapFrameOutlineSize = 5.f;

		// Crearea propriu-zisa
		m_minimapFrame = std::make_unique<Polygon2d>(
			glm::vec2{
				m_miniMap->GetX() - m_minimapFrameOutlineSize,
				m_miniMap->GetY() - m_minimapFrameOutlineSize},
			m_miniMap->GetWidth() + m_minimapFrameOutlineSize * 2.f,
			m_miniMap->GetHeight() + m_minimapFrameOutlineSize * 2.f,
			m_minmapFrameColor);
	}

private:
	utils::LogicSpace m_NDCSpace;
	utils::LogicSpace m_logicSpace;

	glm::mat3 m_transformMatrix;

	std::unique_ptr<Shader> m_shader;

	std::shared_ptr<Speedometer> m_speedometer;
	std::shared_ptr<MiniMap> m_miniMap;

	std::unique_ptr<Polygon2d> m_minimapFrame;
	Color m_minmapFrameColor;
	float m_minimapFrameOutlineSize;
};