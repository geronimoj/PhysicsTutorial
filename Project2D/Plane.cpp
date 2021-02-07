#include <Gizmos.h>
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

	float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

	float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));

	float mass0 = 1.0f / (1.0f / actor2->GetMass() + (r * r) / actor2->GetMoment());

	float j = -(1 + elasticity) * velocityIntoPlane * mass0;

	glm::vec2 force = m_normal * j;

	actor2->ApplyForce(force, contact - actor2->GetPosition());

	float pen = glm::dot(contact, m_normal) - m_distanceToOrigin;
	PhysicsScene::ApplyContactForces(actor2, nullptr, m_normal, pen);
}