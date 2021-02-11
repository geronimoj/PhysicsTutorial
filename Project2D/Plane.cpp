#include <Gizmos.h>
#include <iostream>
#include "Plane.h"
#include "PhysicsScene.h"

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour, float elasticity, float staticFriction, float kinematicFriction)
	: PhysicsObject(ShapeType::PLANE, elasticity, staticFriction, kinematicFriction), m_normal(glm::normalize(normal)), m_distanceToOrigin(distance), m_colour(colour)
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

	float friction = actor2->GetVelocity() == glm::vec2(0,0) ? m_staticFrictionCo + actor2->GetStaticFriction() : m_kinematicFrictionCo + actor2->GetKinematicFriction(); //todo setup fiction co-efficients

	float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

	float r = glm::dot(localContact, perp);

	float mass0 = PhysicsScene::GetMass0(*actor2, r);

	float j = -(1 + elasticity) * velocityIntoPlane * mass0;

	glm::vec2 force = m_normal * j;

	glm::vec2 preVel = actor2->GetVelocity();
	float kePre = 0.5f * actor2->GetMass() * (glm::length(preVel) * glm::length(preVel)) + 0.5f * actor2->GetMoment() * (actor2->GetAngularVelocity() * actor2->GetAngularVelocity());
	//Apply the vertical bounce force off of the plane
	actor2->ApplyForce(force, localContact);
	relativeVelocity = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
	//Get the acceleration over this time skip
	float accel = glm::length(actor2->GetVelocity() - preVel);

	glm::vec2 velocityAlongPlane = perp * glm::dot(relativeVelocity, perp);
	glm::vec2 normalForce = -m_normal * (actor2->GetMass() * accel);
	//Stored as a float because friction force will always be opposed to objects motion
	float frictionForce = friction * glm::length(normalForce);
	//Recalculate the mass0 for the frictional force
	r = glm::dot(localContact, m_normal);
	mass0 = PhysicsScene::GetMass0(*actor2, r);

	actor2->ApplyForce(frictionForce < glm::length(velocityAlongPlane) ? 
		//If friction is smaller use frictionForce
		frictionForce * -glm::normalize(velocityAlongPlane) * mass0
		//If velocity is smaller use velocity
		: -velocityAlongPlane * mass0, localContact);

	float postVel = glm::length(actor2->GetVelocity());
	float kePost = 0.5f * actor2->GetMass() * (postVel * postVel) + 0.5f * actor2->GetMoment() * (actor2->GetAngularVelocity() * actor2->GetAngularVelocity());
	float delta = kePost - kePre;
	if (delta > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greather than 1%. Difference is:" << std::endl;

	float pen = glm::dot(contact, m_normal) - m_distanceToOrigin;
	PhysicsScene::ApplyContactForces(actor2, nullptr, m_normal, pen);
}