#include <iostream>
#include "Rigidbody.h"
#include "PhysicsScene.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, bool isKinematic, glm::vec2 velocity, float orientation, float mass, float elasticity, float angularVelocity, float linearDrag, float angularDrag, float staticFriction, float kinematicFriction)
	: PhysicsObject(shapeID, elasticity, staticFriction, kinematicFriction), m_position(position), m_isKinematic(isKinematic), m_velocity(velocity), m_orientation(orientation), m_mass(mass), m_angularVelocity(angularVelocity), m_moment(0), m_linearDrag(linearDrag), m_angularDrag(angularDrag)
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
		float mass1 = PhysicsScene::GetMass0(*this, r1);
		float mass2 = PhysicsScene::GetMass0(*actor2, r2);

		glm::vec2 preVel1 = GetVelocity();
		glm::vec2 preVel2 = actor2->GetVelocity();

		float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

		glm::vec2 force = (1 + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;
		//Apply elastic force
		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->GetPosition());

		//FRICTION
		r1 = glm::dot(contact - GetPosition(), normal);
		r2 = glm::dot(contact - actor2->GetPosition(), normal);
		//Recalculate the mass at the point of collision
		mass1 = PhysicsScene::GetMass0(*this, r1);
		mass2 = PhysicsScene::GetMass0(*actor2, r2);
		//If neither of us are moving, then use static friction otherwise use kinematic friction
		float friction = actor2->GetVelocity() == glm::vec2(0, 0) && GetVelocity() == glm::vec2(0,0) ? m_staticFrictionCo + actor2->GetStaticFriction() : m_kinematicFrictionCo + actor2->GetKinematicFriction();
		glm::vec2 perpForce1 = perp * glm::dot(GetVelocity() + GetAngularVelocity() * perp, perp);
		glm::vec2 perpForce2 = perp * glm::dot(actor2->GetVelocity() + actor2->GetAngularVelocity() * perp, perp);
		//The normal force will be their combined forces pushing into each other
		glm::vec2 normalForce = -normal * (GetMass() * glm::length(GetVelocity() - preVel1));
		normalForce += -normal * (actor2->GetMass() * glm::length(actor2->GetVelocity() - preVel2));
		float frictionForce = friction * glm::length(normalForce);
		//Apply frictional forces
		ApplyForce(frictionForce < glm::length(perpForce1) ?
			//If friction is smaller use frictionForce
			frictionForce * -glm::normalize(perpForce1) * mass1
			//If velocity is smaller use velocity
			: -perpForce1 * mass1, contact - GetPosition());

		actor2->ApplyForce(frictionForce < glm::length(perpForce2) ?
			//If friction is smaller use frictionForce
			frictionForce * -glm::normalize(perpForce2) * mass2
			//If velocity is smaller use velocity
			: -perpForce2 * mass2, contact - actor2->GetPosition());

		
		if (pen > 0)
			PhysicsScene::ApplyContactForces(this, actor2, normal, pen);
	}
}

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * m_mass * (glm::length(m_velocity) * glm::length(m_velocity));
}
