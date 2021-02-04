#include "Rigidbody.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass) : PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_orientation(orientation), m_mass(mass)
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	//Multiply gravity by mass to hand it in as a force. This is then accounted for in ApplyForce
	ApplyForce(gravity * m_mass * timeStep);
}

void Rigidbody::ApplyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::ApplyForceToActor(Rigidbody* actor2, glm::vec2 force)
{
	m_velocity += force / m_mass;

	actor2->ApplyForce(-force);
}
