#include <Gizmos.h>
#include <iostream>
#include "Plane.h"
#include "PhysicsScene.h"
#define INFERIOR


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
	glm::vec2 expectedVelocity(relativeVelocity);

	glm::vec2 preVel = actor2->GetVelocity();
	float kePre = getEnergy();
	float velocityIntoPlane = glm::dot(relativeVelocity, m_normal);

	float friction = actor2->GetVelocity() == glm::vec2(0,0) ? m_staticFrictionCo + actor2->GetStaticFriction() : m_kinematicFrictionCo + actor2->GetKinematicFriction();
	friction /= 2;

	float r = 0;
	float mass0 = 0;

	float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

	//Elasticity
	//Get the effective mass of the contact point
	r = glm::dot(localContact, perp);
	mass0 = PhysicsScene::GetMass0(*actor2, r);
	//Calculate the magnitude of force to apply
	float j = -(1 + elasticity) * velocityIntoPlane * mass0;
	//Give the force a direction
	glm::vec2 force = m_normal * j;

#ifdef INSTANT
	expectedVelocity += force / mass0;
#endif
#ifdef INFERIOR
	actor2->ApplyForce(force, localContact);
#endif // 

	//Apply the vertical bounce force off of the plane
	//actor2->ApplyForce(force, localContact);

	//Friction
#ifdef INFERIOR
	relativeVelocity = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
#endif
	//Get the acceleration over this time skip
	float accel = glm::length(preVel);

	glm::vec2 velocityAlongPlane = perp * glm::dot(relativeVelocity, perp);
	glm::vec2 normalForce = -m_normal * (actor2->GetMass() * accel);
	//Stored as a float because friction force will always be opposed to objects motion
	float frictionMax = friction * glm::length(normalForce);
	//Recalculate the mass0 for the frictional force
	r = glm::dot(localContact, m_normal);
	mass0 = PhysicsScene::GetMass0(*actor2, r);
	//Get the friction force
	glm::vec2 fForce = frictionMax < glm::length(velocityAlongPlane) ?
		//If friction is smaller use frictionForce
		frictionMax * -glm::normalize(velocityAlongPlane) * mass0
		//If velocity is smaller use velocity
		: -velocityAlongPlane * mass0;

#ifdef INFERIOR
	actor2->ApplyForce(fForce, localContact);
#endif // 

#ifdef INSTANT
	expectedVelocity += fForce / mass0;
	//Calculate the finalVelocity, basically how much we need to accelerate the point to be equal to expectedVelocity
	glm::vec2 finalVelocity = expectedVelocity - relativeVelocity;
	glm::vec2 finalPerpNorm = glm::normalize(glm::vec2(finalVelocity.y, -finalVelocity.x));
	r = glm::dot(localContact, finalPerpNorm);
	mass0 = PhysicsScene::GetMass0(*actor2, r);

	actor2->ApplyForce(finalVelocity * mass0, localContact);
#endif

	//actor2->ApplyForce(frictionForce < glm::length(velocityAlongPlane) ? 
	//	//If friction is smaller use frictionForce
	//	frictionForce * -glm::normalize(velocityAlongPlane) * mass0
	//	//If velocity is smaller use velocity
	//	: -velocityAlongPlane * mass0, localContact);
	relativeVelocity = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);

	float postVel = glm::length(actor2->GetVelocity());
	float kePost = getEnergy();
	float delta = kePost - kePre;
	if (delta > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greather than 1%. Difference is:" << std::endl;

	float pen = glm::dot(contact, m_normal) - m_distanceToOrigin;
	PhysicsScene::ApplyContactForces(actor2, nullptr, m_normal, pen);
}