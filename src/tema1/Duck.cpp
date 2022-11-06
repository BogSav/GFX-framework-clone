#include "Duck.hpp"

Duck::Duck(TranformUtils::LogicSpace logicSpace, TranformUtils::ViewportSpace viewPort)
	:
	Object(logicSpace, viewPort),
	m_randomEngine(std::random_device{}()),
	m_scale(5),
	m_flyingSpeed(10),
	m_randomStartPositionGenerator(
		logicSpace.GetX() + 15/100*logicSpace.GetWidth(), 
		logicSpace.GetRightX() - 15/100*logicSpace.GetWidth()),
	m_randomStartAngleGenerator(
		MySafeGeometry::MyDeg2Rad(30.f),
		MySafeGeometry::MyDeg2Rad(150.f)),
	m_randomChancesForDirectionChange(40, 100)
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
		Colors::YELLOW, -0.1f));
	m_components.emplace("left-wing", new Triangle("left-wing",
		{ 0.271, 0.519 },
		{ 0.04, 0.893 },
		{ 0.330, 0.695 },
		Colors::YELLOW, -0.1f));

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
	m_position[1] = 3/100*logicSpace.GetHeight() + logicSpace.GetY();

	float theta;
	do {
		theta = m_randomStartAngleGenerator(m_randomEngine);
		m_flyingDirection[0] = glm::cosf(theta);
		m_flyingDirection[1] = glm::sinf(theta);
	}
	while (MySafeGeometry::MyGetAngleBetween(m_flyingDirection, {0,1,0}) < MySafeGeometry::MyDeg2Rad(40));

	m_modelOrientation = theta > MySafeGeometry::MyDeg2Rad(90.f) ? -1 : 1;
}

void Duck::Update(float deltaTime)
{
	m_timeBeingASlave += deltaTime;

	if (m_timeBeingASlave > m_slaveryTime)
		SetFree();

	this->UpdatePosition(deltaTime);
	this->UpdateAnimation(deltaTime);
}

void Duck::UpdateAnimation(float deltaTime)
{
	m_modelMatrix *= TranformUtils::Scale(m_scale, m_scale);
	this->UpdateFlyAnimation(deltaTime);
}

void Duck::UpdatePosition(float deltaTime)
{
	if (!m_isDead)
	{
		m_position += m_flyingDirection * m_flyingSpeed * deltaTime;
	}
	else
	{
		if (m_deadDramaticRotationAngle > MySafeGeometry::MyDeg2Rad(90.f))
		{
			m_position += glm::vec3{ 0, -1, 0 } * m_deadFallingSpeed * deltaTime;
		}
		else
		{
			m_deadDramaticRotationAngle += m_deadDramaticRotationSpeed * deltaTime;
		}
	}
	glm::mat3 logicTransformMatrix = m_VLMatrix;
	logicTransformMatrix *= TranformUtils::Translate(m_position[0] + 0.4, m_position[1] + 0.4);
	logicTransformMatrix *= TranformUtils::Rotate(-m_modelOrientation * m_deadDramaticRotationAngle);
	logicTransformMatrix *= TranformUtils::ReflectionMatrixOY(m_modelOrientation);
	//logicTransformMatrix *= TranformUtils::ReflectionMatrixOX(m_nuVreauSaFaAsta);
	logicTransformMatrix *= TranformUtils::Translate(-0.4, -0.4);
	m_modelMatrix = logicTransformMatrix;
}

void Duck::UpdateFlyAnimation(float deltaTime)
{
	m_leftWingModelMatrix = m_modelMatrix;
	m_rightWingModelMatrix = m_modelMatrix;
	if (m_isDead)
		return;

	// Right wing rotation calculation
	m_rightWingRotationAngle += m_rightWingRotationAngularSpeed * deltaTime * m_rightWingRotationDirection;

	if (m_rightWingRotationAngle > std::numbers::pi_v<float> / 4)
		m_rightWingRotationDirection = -1;
	else if (m_rightWingRotationAngle < MySafeGeometry::MyDeg2Rad(-20))
		m_rightWingRotationDirection = 1;
	
	m_rightWingModelMatrix *= TranformUtils::Translate(0.480f, 0.350f);
	m_rightWingModelMatrix *= TranformUtils::Rotate(m_rightWingRotationAngle);
	m_rightWingModelMatrix *= TranformUtils::Translate(-0.480f ,-0.350f);

	// Left wing rotation calculation
	m_leftWingRotationAngle += m_leftWingRotationAngularSpeed * deltaTime * m_leftWingRotationDirection;
	
	if (m_leftWingRotationAngle > std::numbers::pi_v<float> / 4)
		m_leftWingRotationDirection = -1;
	else if (m_leftWingRotationAngle < MySafeGeometry::MyDeg2Rad(-20.f))
		m_leftWingRotationDirection = 1;

	m_leftWingModelMatrix *= TranformUtils::Translate(0.250f, 0.550f);
	m_leftWingModelMatrix *= TranformUtils::Rotate(-m_leftWingRotationAngle);
	m_leftWingModelMatrix *= TranformUtils::Translate(-0.250f, -0.550f);
}

void Duck::ForceRenderByCustomModelMatrix(Shader* shader, glm::mat3 modelMatrix, const gfxc::Camera* const camera)
{
	for (auto& current : m_components)
		current.second->Render(shader, modelMatrix, camera);
	m_modelMatrix = modelMatrix;
}

void Duck::Render(Shader* shader, const gfxc::Camera* const camera)
{
	for (auto& current : m_components)
	{
		if(current.first != "right-wing" && current.first != "left-wing")
			current.second->Render(shader, m_modelMatrix, camera);
	}

	m_components["right-wing"]->Render(shader, m_rightWingModelMatrix, camera);
	m_components["left-wing"]->Render(shader, m_leftWingModelMatrix, camera);
}

BoundingBox Duck::GetBoundingBox() const
{
	return m_bbox.GetBoundingBoxTransformed(m_modelMatrix);
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
		return { true, MySafeGeometry::MyGetAngleBetween(m_flyingDirection, glm::vec3{-1,0,0}), CollisionUtils::LEFT_WALL };
	}
	if (duckBbox.GetBottomLeftCorner()[1] < m_viewPort.GetY())
	{
		return { true, MySafeGeometry::MyGetAngleBetween(m_flyingDirection, glm::vec3{0,-1,0}), CollisionUtils::BOTTOM_WALL };
	}
	if (duckBbox.GetUpperRightCorner()[0] > m_viewPort.GetRightX())
	{
		return { true, MySafeGeometry::MyGetAngleBetween(m_flyingDirection, glm::vec3{1,0,0}), CollisionUtils::RIGHT_WALL };
	}
	if (duckBbox.GetUpperRightCorner()[1] > m_viewPort.GetUpperY())
	{
		return { true, MySafeGeometry::MyGetAngleBetween(m_flyingDirection, glm::vec3{0,1,0}), CollisionUtils::UPPER_WALL };
	}

	return { false, 0, CollisionUtils::NO_COLLISION };
}

void Duck::CollisionDetectAndAct()
{
	if (IsDead() || IsFree())
		return;

	CollisionUtils::CollInfo collInfo = this->GetCollisionInfo();

	if (collInfo.collisionAngle < std::numbers::pi_v<float> / 2)
	{
		switch (collInfo.collisionType)
		{
		case CollisionUtils::LEFT_WALL:
			m_flyingDirection = MySafeGeometry::MyReflectByTheNormal({ 1, 0, 0 }, m_flyingDirection);
			m_modelOrientation = 1;
			break;
		case CollisionUtils::RIGHT_WALL:
			m_flyingDirection = MySafeGeometry::MyReflectByTheNormal({ -1, 0, 0 }, m_flyingDirection);
			m_modelOrientation = -1;
			break;
		case CollisionUtils::UPPER_WALL:
			m_nuVreauSaFaAsta = -1;
			m_flyingDirection = MySafeGeometry::MyReflectByTheNormal({ 0, -1, 0 }, m_flyingDirection);
			break;
		case CollisionUtils::BOTTOM_WALL:
			m_nuVreauSaFaAsta = 1;
			m_flyingDirection = MySafeGeometry::MyReflectByTheNormal({ 0, 1, 0 }, m_flyingDirection);
			break;
		}
	}
}

bool Duck::GotShot(glm::vec2 shotPoint)
{
	if (GetBoundingBox().IsInside(shotPoint))
	{
		m_nrOfShots++;
		switch (m_duckDificulty)
		{
		case 1:
		case 2:
		case 3:
			if (m_nrOfShots >= 1)
				m_isDead = true;
			break;
		case 4:
			if (m_nrOfShots >= 2)
				m_isDead = true;
			break;
		case 5:
			if (m_nrOfShots >= 3)
				m_isDead = true;
			break;
		}
		return true;
	}

	return false;
}
