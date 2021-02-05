#pragma once

#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, float angularVelocity, float linearDrag, float angularDrag);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2 * collisionNormal = nullptr);

	glm::vec2 GetPosition() { return m_position; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetOrientation() { return m_orientation; }
	float GetMass() { return m_mass; }
	float GetMoment() { return m_moment; }
	float GetAngularVelocity() { return m_angularVelocity; }
	float GetKineticEnergy();

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation;
	float m_angularVelocity;
	float m_moment;
	float m_linearDrag;
	float m_angularDrag;
};

