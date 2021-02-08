#pragma once

#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, bool isKinematic, glm::vec2 velocity, float orientation, float mass, float elasticity, float angularVelocity, float linearDrag, float angularDrag);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2 * collisionNormal = nullptr, float pen = 0);

	void SetPosition(glm::vec2 position) { m_position = position; }
	void SetKinematic(bool state) { m_isKinematic = state; }

	glm::vec2 GetPosition() const { return m_position; }
	glm::vec2 GetVelocity() const { return m_velocity; }
	virtual glm::vec2 ToWorld(glm::vec2 offset) { return GetPosition() + offset; }
	float GetOrientation() const { return m_orientation; }
	float GetMass() const { return m_isKinematic ? INT_MAX : m_mass; }
	float GetMoment() const { return m_isKinematic ? INT_MAX : m_moment; }
	float GetAngularVelocity() const { return m_angularVelocity; }
	float GetKineticEnergy();
	bool IsKinematic() { return m_isKinematic; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation;
	float m_angularVelocity;
	float m_moment;
	float m_linearDrag;
	float m_angularDrag;
	bool m_isKinematic;
};

