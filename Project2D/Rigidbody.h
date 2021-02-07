#pragma once

#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, float elasticity, float angularVelocity, float linearDrag, float angularDrag);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2 * collisionNormal = nullptr, float pen = 0);

	void SetPosition(glm::vec2 position) { m_position = position; }

	glm::vec2 GetPosition() const { return m_position; }
	glm::vec2 GetVelocity() const { return m_velocity; }
	float GetOrientation() const { return m_orientation; }
	float GetMass() const { return m_mass; }
	float GetMoment() const { return m_moment; }
	float GetAngularVelocity() const { return m_angularVelocity; }
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

