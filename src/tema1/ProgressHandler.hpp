#pragma once

#include "Object.hpp"

class ProgressHandler : public Object
{
public:
	ProgressHandler(TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace viewPortSpace, float limit)
		:
		Object(logicSpace, viewPortSpace),
		m_limit(limit)
	{
		m_handlerBox = std::make_unique<Rectangle>("box",
			glm::vec2{ 0, 0 },
			5.f,
			1.f,
			Colors::CYAN,
			0.7f,
			true);
		m_handler = std::make_unique<Rectangle>("handler",
			glm::vec2{ 0, 0 },
			.1f,
			1.f,
			Colors::CYAN,
			0.7f);
		m_currentProgress = 0;
	}

	void Render(Shader* shader, const gfxc::Camera* const camera)
	{
		glm::mat3 VLMatrix = TranformUtils::VisualizationTransf2D(m_logicSpace, m_viewPort);
		m_handlerBox->Render(shader, VLMatrix, camera);

		float scaleX = 50.f / m_limit * m_currentProgress;

		m_modelMatrix = VLMatrix * TranformUtils::Translate(0, 0);
		m_modelMatrix *= TranformUtils::Scale(scaleX, 1.f);

		m_handler->Render(shader, m_modelMatrix, camera);
	}

	void ResetLimit(float limit)
	{
		m_limit = limit;
		m_currentProgress = 0;
	}

	void SetProgress(float currentProgress)
	{
		m_currentProgress = currentProgress;
	}
private:
	std::unique_ptr<GeometryObject> m_handlerBox;
	std::unique_ptr<GeometryObject> m_handler;

	float m_limit;
	float m_currentProgress;
};