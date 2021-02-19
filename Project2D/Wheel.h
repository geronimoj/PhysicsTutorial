#pragma once
#include "Sphere.h"

class Wheel : public Sphere
{
public:
	Wheel(glm::vec2 position, float mass, float radius, float angularAcceleration, float angularVelocityLimit, glm::vec4 colour, unsigned int layer, float linearDrag = 0, float angularDrag = 0, float elasticity = 1, float staticFriction = 1, float kinematicFriction = 1);
	~Wheel();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

protected:
	float m_angularAcceleration;
	float m_angularVelocityLimit;
};

