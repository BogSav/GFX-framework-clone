#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "tema2/Utilities/Camera.hpp"
#include "tema2/Utilities/Colors.hpp"
#include "tema2/Utilities/Utilities.hpp"

class GeometryObject3d
{
public:
	GeometryObject3d() = default;
	GeometryObject3d(const Shader* const shader, CustomCamera* const camera, Color color);

	void Render(const glm::mat4& modelMatrix) const;

	void Render(
		const glm::mat4& modelMatrix, const glm::vec3& carPosition, const float& coefficient) const;

	void Render(
		const glm::mat4& modelMatrix,
		const glm::vec3& carPosition,
		const glm::vec3& eyePosition,
		const float& coefficient,
		const std::vector<const class LightSourceAdapter*>&
			lightingComponents) const;

	void Render(const glm::mat4& modelMatrix, const Color& color) const;

	void Render(
		const Shader* const shader,
		const CustomCamera* const camera,
		const glm::mat4& modelMatrix) const;

	void Render(
		const Shader* const shader,
		const CustomCamera* const camera,
		const glm::mat4& modelMatrix,
		const Color color) const;

private:
	void SendDataToShader(const glm::mat4& modelMatrix) const;

protected:
	void CalculateVerticesNormals(
		std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);

protected:
	std::unique_ptr<Mesh> m_mesh = nullptr;
	Color m_color;

	const Shader* m_shader = nullptr;
	CustomCamera* m_camera = nullptr;
};