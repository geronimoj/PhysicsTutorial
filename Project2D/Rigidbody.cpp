#include <iostream>
#include "Rigidbody.h"
#include "PhysicsScene.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, bool isKinematic, glm::vec2 velocity, float orientation, float mass, float elasticity, float angularVelocity, float linearDrag, float angularDrag) 
	: PhysicsObject(shapeID, elasticity), m_position(position), m_isKinematic(isKinematic), m_velocity(velocity), m_orientation(orientation), m_mass(mass), m_angularVelocity(angularVelocity), m_moment(0), m_linearDrag(linearDrag), m_angularDrag(angularDrag)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKinematic)
	{
		m_velocity = glm::vec2(0, 0);
		m_angularVelocity = 0;
		return;
	}
	//Apply drag before moving
	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	//Ensure we don't end up flipping the vectors or negative numbers
	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		if (glm::length(m_velocity) < glm::length(gravity) * m_linearDrag * timeStep)
			m_velocity = glm::vec2(0, 0);

	if (glm::abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
		m_angularVelocity = 0;

	//Move the object
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

void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal, float pen)
{
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->GetPosition() - GetPosition());
	glm::vec2 perp(normal.y, -normal.x);

	float r1 = glm::dot(contact - GetPosition(), -perp);
	float r2 = glm::dot(contact - actor2->GetPosition(), perp);

	float v1 = glm::dot(GetVelocity(), normal) - r1 * GetAngularVelocity();
	float v2 = glm::dot(actor2->GetVelocity(), normal) + r2 * actor2->GetAngularVelocity();

	if (v1 > v2)
	{
		float mass1 = 1.0f / (1.0f / GetMass() + (r1 * r1) / GetMoment());
		float mass2 = 1.0f / (1.0f / actor2->GetMass() + (r2 * r2) / actor2->GetMoment());

		float friction = 1; //ToDo setup fiction coefficients
		glm::vec2 perpForce1 = perp * glm::dot(GetVelocity() + GetAngularVelocity() * perp, perp);
		glm::vec2 perpForce2 = perp * glm::dot(actor2->GetVelocity() + actor2->GetAngularVelocity() * perp, perp);

		ApplyForce(-friction * perpForce1, contact - GetPosition());
		actor2->ApplyForce(-friction * perpForce2, contact - actor2->GetPosition());

		float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;
		 
		glm::vec2 force = (1 + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->GetPosition());
		
		if (pen > 0)
			PhysicsScene::ApplyContactForces(this, actor2, normal, pen);
	}
}

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * m_mass * (glm::length(m_velocity) * glm::length(m_velocity));
}
