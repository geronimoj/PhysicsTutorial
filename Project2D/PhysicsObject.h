#pragma once
#include <glm/glm.hpp>

enum class ShapeType
{
	SPRING = -1,
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
	SHAPE_COUNT = 3
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType shapeID, float elasticity) : m_shapeID(shapeID), m_elasticity(elasticity) {}

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw() = 0;
	virtual void ResetPosition() {};

	ShapeType GetShapeID() { return m_shapeID; }

	float GetElasticity() { return m_elasticity; }

	static const float MIN_LINEAR_THRESHOLD;
	static const float MIN_ANGULAR_THRESHOLD;
protected:
	ShapeType m_shapeID;
	float m_elasticity;
};

