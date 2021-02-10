#include <Gizmos.h>
#include <iostream>
#include "Plane.h"
#include "PhysicsScene.h"

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

	glm::vec2 relativeVelocity = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);

	float velocityIntoPlane = glm::dot(relativeVelocity, m_normal);

	glm::vec2 perp(m_normal.y, -m_normal.x);
	glm::vec2 contactDirection = glm::normalize(glm::vec2(localContact.y, -localContact.x));
	glm::vec2 velocityAlongPlane = perp * glm::dot(relativeVelocity, perp);

	float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

	float r = glm::dot(localContact, perp);

	float mass0 = 1.0f / (1.0f / actor2->GetMass() + (r * r) / actor2->GetMoment());

	float j = -(1 + elasticity) * velocityIntoPlane * mass0;

	glm::vec2 force = m_normal * j;

	float preVel = glm::length(actor2->GetVelocity());
	float kePre = 0.5f * actor2->GetMass() * (preVel * preVel) + 0.5f * actor2->GetMoment() * (actor2->GetAngularVelocity() * actor2->GetAngularVelocity());

	float friction = 1.0f; //todo setup fiction co-efficients
	//Issues, boxes gain rotation and speed allowing them to gain infinite velocity from collisions
	actor2->ApplyForce(-friction * velocityAlongPlane, localContact);

	actor2->ApplyForce(force, localContact);

	float postVel = glm::length(actor2->GetVelocity());
	float kePost = 0.5f * actor2->GetMass() * (postVel * postVel) + 0.5f * actor2->GetMoment() * (actor2->GetAngularVelocity() * actor2->GetAngularVelocity());
	float delta = kePost - kePre;
	if (delta > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greather than 1%. Difference is:" << delta <<  "	Velocity Gain: " << (((delta * 2) / actor2->GetMass()) / postVel) - preVel << std::endl;

	float pen = glm::dot(contact, m_normal) - m_distanceToOrigin;
	PhysicsScene::ApplyContactForces(actor2, nullptr, m_normal, pen);
}