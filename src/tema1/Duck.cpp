#include "Duck.hpp"
#include <iostream>

Duck::Duck(TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace viewPort)
	:
	m_randomEngine(std::random_device{}()),
	m_logicSpace(logicSpace),
	m_viewPort(viewPort),
	m_scale(5),
	m_flyingSpeed(20),
	m_randomStartPositionGenerator(
		logicSpace.GetX() + 15/100*logicSpace.GetWidth(), 
		logicSpace.GetRightX() - 15/100*logicSpace.GetWidth()),
	m_randomStartAngleGenerator(
		std::numbers::pi_v<float> / 4,
		3 * std::numbers::pi_v<float> / 4)
{
	m_components.emplace("left-leg", new Rectangle("left-leg",
		{ 0.053, 0.214 },
		{ 0.108, 0.291 },
		{ 0.167, 0.237 },
		{ 0.119,0.160 },
		Colors::YELLOW));
	m_components.emplace("right-leg", new Rectangle("right-leg",
		{ 0.168, 0.110 },
		{ 0.230, 0.186 },
		{ 0.289,0.132 },
		{ 0.236, 0.054 },
		Colors::YELLOW));

	m_components.emplace("body", new Rectangle("body",
		{ 0.073, 0.325 },
		{ 0.34, 0.702 },
		{ 0.585,0.528 },
		{ 0.352, 0.072 },
		Colors::GREEN));

	m_components.emplace("right-wing", new Triangle("right-wing",
		{ 0.479, 0.387 },
		{ 0.58, 0.518 },
		{ 0.894, 0.254 },
		Colors::YELLOW, -0.1));
	m_components.emplace("left-wing", new Triangle("left-wing",
		{ 0.271, 0.519 },
		{ 0.04, 0.893 },
		{ 0.330, 0.695 },
		Colors::YELLOW, -0.1));

	m_components.emplace("big-eye", new Circle("big-eye", 10, Colors::RED, 0.2f, { 0.566, 0.747, 0 }));
	m_components.emplace("small-eye", new Circle("small-eye", 10, Colors::WHITE, 0.10f, { 0.566, 0.824, 0 }, .1f));
	m_components.emplace("iris", new Circle("iris", 10, Colors::BLACK, 0.05f, { 0.566, 0.835, 0 }, .2f));

	m_components.emplace("beak", new Triangle("beak",
		{ 0.734, 0.735 },
		{ 0.706, 0.834 },
		{ 0.984, 0.873 },
		Colors::YELLOW));

	this->CalculateBoundingBox();
	m_VLMatrix = TranformUtils::VisualizationTransf2DUnif(m_logicSpace, m_viewPort);

	m_position[0] = m_randomStartPositionGenerator(m_randomEngine);
	m_position[1] = 1;

	float theta = m_randomStartAngleGenerator(m_randomEngine);
	m_flyingDirection[0] = glm::cosf(theta);
	m_flyingDirection[1] = glm::sinf(theta);
}

void Duck::Update(float deltaTime)
{
	this->UpdatePosition(deltaTime);
	this->UpdateAnimation(deltaTime);
}

void Duck::UpdateAnimation(float deltaTime)
{
	m_duckModelMatrix *= TranformUtils::Scale(m_scale, m_scale);

	this->UpdateFlyAnimation(deltaTime);
}

void Duck::UpdatePosition(float deltaTime)
{
	m_position += m_flyingDirection * m_flyingSpeed * deltaTime;

	glm::mat3 logicTransformMatrix = glm::mat3(1);
	logicTransformMatrix *= TranformUtils::Translate(m_position[0], m_position[1]);

	m_duckModelMatrix = m_VLMatrix * logicTransformMatrix;
}

void Duck::UpdateFlyAnimation(float deltaTime)
{
	m_leftWingModelMatrix = m_duckModelMatrix;
	m_rightWingModelMatrix = m_duckModelMatrix;
	if (m_wasShot)
		return;

	// Right wing rotation calculation
	m_rightWingRotationAngle += m_rightWingRotationAngularSpeed * deltaTime * m_rightWingRotationDirection;

	if (m_rightWingRotationAngle > std::numbers::pi_v<float> / 4)
		m_rightWingRotationDirection = -1;
	else if (m_rightWingRotationAngle < MySafeGeometry::MyDeg2Rad(-20))
		m_rightWingRotationDirection = 1;
	
	m_rightWingModelMatrix *= TranformUtils::Translate(0.480, 0.350);
	m_rightWingModelMatrix *= TranformUtils::Rotate(m_rightWingRotationAngle);
	m_rightWingModelMatrix *= TranformUtils::Translate(-0.480 ,-0.350);

	// Left wing rotation calculation
	m_leftWingRotationAngle += m_leftWingRotationAngularSpeed * deltaTime * m_leftWingRotationDirection;
	
	if (m_leftWingRotationAngle > std::numbers::pi_v<float> / 4)
		m_leftWingRotationDirection = -1;
	else if (m_leftWingRotationAngle < MySafeGeometry::MyDeg2Rad(-20))
		m_leftWingRotationDirection = 1;

	m_leftWingModelMatrix *= TranformUtils::Translate(0.250, 0.550);
	m_leftWingModelMatrix *= TranformUtils::Rotate(-m_leftWingRotationAngle);
	m_leftWingModelMatrix *= TranformUtils::Translate(-0.250, -0.550);
}

void Duck::ForceRenderByCustomModelMatrix(Shader* shader, glm::mat3 modelMatrix, const gfxc::Camera* const camera)
{
	for (auto& current : m_components)
		current.second->Render(shader, modelMatrix, camera);
	m_duckModelMatrix = modelMatrix;
}

void Duck::Render(Shader* shader, const gfxc::Camera* const camera)
{
	for (auto& current : m_components)
	{
		if(current.first != "right-wing" && current.first != "left-wing")
			current.second->Render(shader, m_duckModelMatrix, camera);
	}

	m_components["right-wing"]->Render(shader, m_rightWingModelMatrix, camera);
	m_components["left-wing"]->Render(shader, m_leftWingModelMatrix, camera);
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

CollisionUtils::CollInfo Duck::GetCollisionInfo()
{
	BoundingBox duckBbox = GetBoundingBox();

	if (duckBbox.GetBottomLeftCorner()[0] < m_viewPort.GetX())
	{
		m_lastCollisionState = true;
		return { true, MySafeGeometry::MyGetAngleBetween(m_flyingDirection, glm::vec3{-1,0,0}), CollisionUtils::LEFT_WALL };
	}
	if (duckBbox.GetBottomLeftCorner()[1] < m_viewPort.GetY())
	{
		m_lastCollisionState = true;
		return { true, MySafeGeometry::MyGetAngleBetween(m_flyingDirection, glm::vec3{0,-1,0}), CollisionUtils::BOTTOM_WALL };
	}
	if (duckBbox.GetUpperRightCorner()[0] > m_viewPort.GetRightX())
	{
		m_lastCollisionState = true;
		return { true, MySafeGeometry::MyGetAngleBetween(m_flyingDirection, glm::vec3{1,0,0}), CollisionUtils::RIGHT_WALL };
	}
	if (duckBbox.GetUpperRightCorner()[1] > m_viewPort.GetUpperY())
	{
		m_lastCollisionState = true;
		return { true, MySafeGeometry::MyGetAngleBetween(m_flyingDirection, glm::vec3{0,1,0}), CollisionUtils::UPPER_WALL };
	}

	return { false, 0, CollisionUtils::NO_COLLISION };
}

bool Duck::GotShot(glm::vec2 shotPoint)
{
	if (GetBoundingBox().IsInside(shotPoint))
	{
		m_wasShot = true;
		return true;
	}
	else
	{
		m_wasShot = false;
		return false;
	}
}

void Duck::CollisionDetectAndAct()
{
	CollisionUtils::CollInfo collInfo = this->GetCollisionInfo();

	if (collInfo.collisionDetected == false)
	{
		m_isInCollision = false;
		m_lastCollisionState = false;
		return;
	}

	if (m_lastCollisionState == m_isInCollision)
		return;

	switch (collInfo.collisionType)
	{
	case CollisionUtils::LEFT_WALL:
		m_flyingDirection = MySafeGeometry::MyReflectByTheNormal({ 1, 0, 0 }, m_flyingDirection);
		break;
	case CollisionUtils::RIGHT_WALL:
		m_flyingDirection = MySafeGeometry::MyReflectByTheNormal({ -1, 0, 0 }, m_flyingDirection);
		break;
	case CollisionUtils::UPPER_WALL:
		m_flyingDirection = MySafeGeometry::MyReflectByTheNormal({ 0, -1, 0 }, m_flyingDirection);
		break;
	case CollisionUtils::BOTTOM_WALL:
		m_flyingDirection = MySafeGeometry::MyReflectByTheNormal({ 0, 1, 0 }, m_flyingDirection);
		break;
	}

	m_isInCollision = true;
}
