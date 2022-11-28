#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace utils
{
inline glm::vec3 GetInterpolatedPoint(
	const glm::vec3& v1, const glm::vec3& v2, const float interpolationPoint)
{
	return v1 + (v2 - v1) * interpolationPoint;
}

template <typename T>
struct VLSpace
{
	VLSpace(T x, T y, T width, T height)
		: x(x), y(y), width(width), height(height), rightX(x + width), upperY(y + height)
	{
	}
	VLSpace() : VLSpace(0, 0, 1, 1) {}

public:
	void SetX(T x)
	{
		this->x = x;
		UpdateParams();
	}
	void SetY(T y)
	{
		this->y = y;
		UpdateParams();
	}
	void SetWidth(T width)
	{
		this->width = width;
		UpdateParams();
	}
	void SetHeight(T height)
	{
		this->height = height;
		UpdateParams();
	}
	T GetRightX() const { return rightX; }
	T GetUpperY() const { return upperY; }
	T GetX() const { return x; }
	T GetY() const { return y; }
	T GetWidth() const { return width; }
	T GetHeight() const { return height; }

private:
	void UpdateParams()
	{
		rightX = x + width;
		upperY = y + height;
	}

private:
	T x;
	T y;
	T width;
	T height;
	T rightX;
	T upperY;
};
typedef VLSpace<float> LogicSpace;
typedef VLSpace<int> ViewportSpace;

// Transformare din sptiu din rhs in spatiul din lhs
inline glm::mat3 VisualizationTransf2d(const utils::LogicSpace& rhs, const utils::LogicSpace& lhs)
{
	float sx, sy, tx, ty;
	sx = lhs.GetWidth() / rhs.GetWidth();
	sy = lhs.GetHeight() / rhs.GetHeight();
	tx = lhs.GetX() - sx * rhs.GetX();
	ty = lhs.GetY() - sy * rhs.GetY();

	return glm::transpose(glm::mat3(sx, 0.0f, tx, 0.0f, sy, ty, 0.0f, 0.0f, 1.0f));
}
inline glm::mat3 Translate2d(float translateX, float translateY)
{
	return glm::transpose(glm::mat3(1, 0, translateX, 0, 1, translateY, 0, 0, 1));
}

inline glm::mat3 Scale2d(float scaleX, float scaleY)
{
	return glm::transpose(glm::mat3(scaleX, 0, 0, 0, scaleY, 0, 0, 0, 1));
}

inline glm::mat3 Rotate2d(float radians)
{
	float sinp = glm::sinf(radians);
	float cosp = glm::cosf(radians);
	return glm::transpose(glm::mat3(cosp, -sinp, 0, sinp, cosp, 0, 0, 0, 1));
}
}  // namespace utils