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
	PhysicsObject(ShapeType shapeID, unsigned int layer, float elasticity, float staticFriction, float kinematicFriction) : m_shapeID(shapeID), m_layer(layer), m_elasticity(elasticity), m_staticFrictionCo(staticFriction), m_kinematicFrictionCo(kinematicFriction) {}

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw() = 0;
	virtual void ResetPosition() {};

	ShapeType GetShapeID() { return m_shapeID; }

	float GetElasticity() { return m_elasticity; }
	float GetStaticFriction() { return m_staticFrictionCo; }
	float GetKinematicFriction() { return m_kinematicFrictionCo; }
	unsigned int GetLayer() const { return m_layer; }

	static const float MIN_LINEAR_THRESHOLD;
	static const float MIN_ANGULAR_THRESHOLD;
	virtual float getEnergy() { return 0; }

protected:
	ShapeType m_shapeID;
	float m_elasticity;
	float m_staticFrictionCo;
	float m_kinematicFrictionCo;
	unsigned int m_layer;
};

