#include <Gizmos.h>
#include <iostream>
#include "Plane.h"
#include "PhysicsScene.h"

float GetMass0(const Rigidbody& actor, float r)
{
	return 1.0f / (1.0f / actor.GetMass() + (r * r) / actor.GetMoment());
}

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour, float elasticity) : PhysicsObject(ShapeType::PLANE, elasticity), m_normal(glm::normalize(normal)), m_distanceToOrigin(distance), m_colour(colour)
{
}

void Plane::Draw()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	//Get a parallel vector against the normal, the direction of the plane
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colourFade = m_colour;
	colourFade.a = 0;

	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.f, m_colour, m_colour, colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.f, start - m_normal * 10.f, m_colour, colourFade, colourFade);
}

void Plane::ResolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
	glm::vec2 localContact = contact - actor2->GetPosition();

	glm::vec2 perp(m_normal.y, -m_normal.x);

	glm::vec2 relativeVelocity = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);

	float velocityIntoPlane = glm::dot(relativeVelocity, m_normal);

	float friction = 1.0f; //todo setup fiction co-efficients

	float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

	float r = glm::dot(localContact, perp);

	float mass0 = GetMass0(*actor2, r);

	float j = -(1 + elasticity) * velocityIntoPlane * mass0;

	glm::vec2 force = m_normal * j;

	float preVel = glm::length(actor2->GetVelocity());
	float kePre = 0.5f * actor2->GetMass() * (preVel * preVel) + 0.5f * actor2->GetMoment() * (actor2->GetAngularVelocity() * actor2->GetAngularVelocity());
	//Apply the vertical bounce force off of the plane
	actor2->ApplyForce(force, localContact);
	relativeVelocity = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);

	glm::vec2 velocityAlongPlane = perp * glm::dot(relativeVelocity, perp);
	glm::vec2 normalForce = -m_normal * (actor2->GetMass() * glm::vec2(0, -100));
	//Stored as a float because friction force will always be opposed to objects motion
	float frictionForce = friction * glm::length(normalForce);
	//Recalculate the mass0 for the frictional force
	r = glm::dot(localContact, m_normal);
	mass0 = GetMass0(*actor2, r);
	//Issues, boxes gain rotation and speed allowing them to gain infinite velocity from collisions
	actor2->ApplyForce(frictionForce < glm::length(relativeVelocity) ? 
		//If friction is smaller use frictionForce
		frictionForce * -glm::normalize(velocityAlongPlane) * mass0
		//If velocity is smaller use velocity
		: -velocityAlongPlane * mass0, localContact);

	float postVel = glm::length(actor2->GetVelocity());
	float kePost = 0.5f * actor2->GetMass() * (postVel * postVel) + 0.5f * actor2->GetMoment() * (actor2->GetAngularVelocity() * actor2->GetAngularVelocity());
	float delta = kePost - kePre;
	if (delta > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greather than 1%. Difference is:" << delta <<  "	Velocity Gain: " << (((delta * 2) / actor2->GetMass()) / postVel) - preVel << std::endl;

	float pen = glm::dot(contact, m_normal) - m_distanceToOrigin;
	PhysicsScene::ApplyContactForces(actor2, nullptr, m_normal, pen);
}