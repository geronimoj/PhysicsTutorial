#include <Gizmos.h>
#include "Spring.h"

Spring::Spring(Rigidbody* body1, Rigidbody* body2, float restLength, glm::vec4 colour, float springCoefficient, float damping, glm::vec2 body1Contact, glm::vec2 body2Contact) 
	: PhysicsObject(ShapeType::SPRING, 0, 0, 0, 0), m_body1(body1), m_body2(body2), m_contact1(body1Contact), m_contact2(body2Contact), m_restLength(restLength),
m_damping(damping), m_springCoefficient(springCoefficient), m_colour(colour)
{
}

Spring::Spring(Rigidbody* body1, Rigidbody* body2, float damping, float springCoefficient)
	: PhysicsObject(ShapeType::SPRING, 0, 0, 0, 0), m_body1(body1), m_body2(body2), m_contact1(glm::vec2(0, 0)), m_contact2(glm::vec2(0,0)), m_damping(damping), m_springCoefficient(springCoefficient), m_colour(glm::vec4(1,1,1,1))
{
	//Set the rest length to be the difference in contact points
	m_restLength = glm::length(m_body1->ToWorld(m_contact1) - m_body2->ToWorld(m_contact2));
}

void Spring::FixedUpdate(glm::vec2 gravity, float timeStep)
{	//Get the contact points in world co-ordinates
	glm::vec2 p2 = m_body2->ToWorld(m_contact2);
	glm::vec2 p1 = m_body1->ToWorld(m_contact1);
	//Get a vector from p1 to p2
	glm::vec2 dist = p2 - p1;

	//Calculate the length of said vector
	float length = sqrtf(dist.x * dist.x + dist.y * dist.y);
	//Calculate the relative velocity
	glm::vec2 relativeVelocity = m_body2->GetVelocity() - m_body1->GetVelocity();
	//Calculate the force to apply to the two bodies.
	glm::vec2 force = dist * m_springCoefficient * (m_restLength - length) - m_damping * relativeVelocity;
	//Apply a cap force to avoid "spaziming" with large values of force
	const float threshold = 1000;
	float forceMag = glm::length(force);
	//Add a maximum force
	if (forceMag > threshold)
		force *= threshold / forceMag;
	//Apply the force
	m_body1->ApplyForce(-force * timeStep, p1 - m_body1->GetPosition());
	m_body2->ApplyForce(force * timeStep, p2 - m_body2->GetPosition());
}

void Spring::Draw()
{	//Draw a line from the contact point to the other one.
	glm::vec2 start = m_body1->ToWorld(m_contact1);
	glm::vec2 end = m_body2->ToWorld(m_contact2) - start;
	aie::Gizmos::add2DLine(start, start + end, m_colour);
}
