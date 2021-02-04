#pragma once
#include <glm/glm.hpp>

enum ShapeType
{
	PLANE,
	SPHERE,
	BOX
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType shapeID) : m_shapeID(shapeID) {}

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw() = 0;
	virtual void ResetPosition() {};

protected:
	ShapeType m_shapeID;
};

