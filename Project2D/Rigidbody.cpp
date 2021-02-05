#include "Rigidbody.h"
#include <iostream>

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, float angularVelocity, float linearDrag, float angularDrag) 
	: PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_orientation(orientation), m_mass(mass), m_angularVelocity(angularVelocity), m_moment(0), m_linearDrag(linearDrag), m_angularDrag(angularDrag)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	//Multiply gravity by mass to hand it in as a force. This is then accounted for in ApplyForce
	ApplyForce(gravity * m_mass * timeStep, glm::vec2(0,0));

	m_orientation += m_angularVelocity * timeStep;
}

void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / GetMass();

	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / GetMoment();
}

void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->GetPosition() - GetPosition());
	glm::vec2 perp(normal.y, -normal.x);

	float r1 = glm::dot(contact - GetPosition(), -perp);
	float r2 = glm::dot(contact - actor2->GetPosition(), perp);

	float v1 = glm::dot(GetVelocity(), normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(actor2->GetVelocity(), normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2)
	{
		float mass1 = 1.0f / (1.0f / GetMass() + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->GetMass() + (r2 * r2) / actor2->GetMoment());


		float elasticity = 1;
		 
		glm::vec2 force = (1 + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->GetPosition());
	}
}

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * m_mass * (glm::length(m_velocity) * glm::length(m_velocity));
}
