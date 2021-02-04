#include <Gizmos.h>
#include "Plane.h"

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour) : PhysicsObject(ShapeType::PLANE), m_normal(normal), m_distanceToOrigin(distance), m_colour(colour)
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

void Plane::ResolveCollision(Rigidbody* actor2)
{
	glm::vec2 relativeVelocity = actor2->GetVelocity();

	float elasticity = 0.9;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), m_normal) / ((1 / actor2->GetMass()));

	glm::vec2 force = m_normal * j;

	actor2->ApplyForce(force);
}
