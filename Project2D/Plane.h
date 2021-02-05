#pragma once

#include "PhysicsObject.h"
#include "Rigidbody.h"

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 normal, float distance, glm::vec4 colour);
	~Plane();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) {}
	virtual void Draw();
	virtual void ResetPosition() {}

	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact);

	glm::vec2 GetNormal() { return m_normal; }
	float GetDistance() { return m_distanceToOrigin; }
	glm::vec4 GetColour() { return m_colour; }

protected:
	glm::vec2 m_normal;
	glm::vec4 m_colour;
	float m_distanceToOrigin;
};

