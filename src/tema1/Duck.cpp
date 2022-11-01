#include "Duck.hpp"

Duck::Duck(glm::vec3 position, float speed, TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace viewPort)
	:
	m_position(position),
	m_flyingSpeed(speed),
	m_logicSpace(logicSpace),
	m_viewPort(viewPort)
{
	m_components["left-leg"] = new Rectangle("left-leg",
		{ 0.053, 0.214 },
		{ 0.108, 0.291 },
		{ 0.167, 0.237 },
		{ 0.119,0.160 },
		Colors::YELLOW);
	m_components["right-leg"] = new Rectangle("right-leg",
		{ 0.168, 0.110 },
		{ 0.230, 0.186 },
		{ 0.289,0.132 },
		{ 0.236, 0.054 },
		Colors::YELLOW);

	m_components["body"] = new Rectangle("body",
		{ 0.073, 0.325 },
		{ 0.34, 0.702 },
		{ 0.585,0.528 },
		{ 0.352, 0.072 },
		Colors::GREEN);

	m_components["right-arm"] = new Triangle("right-arm",
		{ 0.51, 0.372 },
		{ 0.58, 0.518 },
		{ 0.894, 0.254 },
		Colors::YELLOW);
	m_components["left-arm"] = new Triangle("left-arm",
		{ 0.239, 0.566 },
		{ 0.04, 0.893 },
		{ 0.330, 0.695 },
		Colors::YELLOW);

	m_components["big-eye"] = new Circle("big-eye", 10, Colors::RED, 0.2f, { 0.566, 0.747, 0 });
	//m_components["small-eye"] = new Circle("small-eye", 10, Colors::WHITE, 0.13f, { 0.566, 0.824, 0 });
	//m_components["iris"] = new Circle("iris", 10, Colors::CYAN, 0.05f, { 0.566, 0.835, 0 });

	m_components["beak"] = new Triangle("beak",
		{ 0.734, 0.735 },
		{ 0.706, 0.834 },
		{ 0.984, 0.873 },
		Colors::YELLOW);

	this->CalculateBoundingBox();
}

void Duck::Update(float deltaTime, TranformUtils::ViewportSpace& , TranformUtils::LogicSpace& )
{

}

void Duck::Render(Shader* shader, const gfxc::Camera* const camera)
{
	for (auto& current : m_components)
		current.second->Render(shader, m_duckModelMatrix, camera);
}

BoundingBox Duck::GetBoundingBox() const
{
	return m_bbox.GetBoundingBoxTransformed(m_duckModelMatrix);
}

void Duck::CalculateBoundingBox()
{
	for (auto& current : m_components)
		this->m_bbox += current.second->GetBoundingBox();
}
