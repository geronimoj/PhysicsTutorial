#pragma once

#include "PhysicsObject.h"
#include "Rigidbody.h"

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 normal, float distance, unsigned int layer, glm::vec4 colour, float elasticity = 1, float staticFriction = 1, float kinematicFriction = 1);
	~Plane();
	
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) {}
	virtual void Draw();
	virtual void ResetPosition() {}

	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact);

	glm::vec2 GetNormal() const { return m_normal; }
	float GetDistance() const { return m_distanceToOrigin; }
	glm::vec4 GetColour() const { return m_colour; }

protected:
	glm::vec2 m_normal;
	glm::vec4 m_colour;
	float m_distanceToOrigin;
};

