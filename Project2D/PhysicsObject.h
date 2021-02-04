#pragma once
#include <glm/glm.hpp>

enum class ShapeType
{
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
	SHAPE_COUNT = 3
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType shapeID) : m_shapeID(shapeID) {}

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw() = 0;
	virtual void ResetPosition() {};

	ShapeType GetShapeID() { return m_shapeID; }
protected:
	ShapeType m_shapeID;
};

