#include "Rigidbody.h"
#include <iostream>

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
{	//Apply force to this rigidbody
	ApplyForce(-force);
	//Apply negative force to the other rigidbody.
	actor2->ApplyForce(force);
}

void Rigidbody::ResolveCollision(Rigidbody* actor2)
{
	glm::vec2 normal = glm::normalize(actor2->GetPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->GetVelocity() - m_velocity;

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / ((1 / m_mass) + (1 / actor2->GetMass()));

	glm::vec2 force = normal * j;

	float kePre = GetKineticEnergy() + actor2->GetKineticEnergy();

	ApplyForceToActor(actor2, force);

	float kePost = GetKineticEnergy() + actor2->GetKineticEnergy();
	if (kePost - kePre > kePost * 0.01f)
		std::cout << "Kinetic Energy loss is greater than 1% in perfect system";
}

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * m_mass * (glm::length(m_velocity) * glm::length(m_velocity));
}
