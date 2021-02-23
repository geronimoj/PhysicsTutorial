#include <Gizmos.h>
#include "Sphere.h"

Sphere::Sphere(glm::vec2 position, bool isKinematic, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, float angularVelocity, unsigned int layer, float linearDrag, float angularDrag, float elasticity, float staticFriction, float kineticFriction)
	: Rigidbody(ShapeType::SPHERE, position, isKinematic, velocity, 0, 0, mass, elasticity, angularVelocity, linearDrag, angularDrag, staticFriction, kineticFriction), m_radius(radius), m_colour(colour)
{	//Calculate the moment = 1/2mr^2
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

void Sphere::Draw()
{	//Calculate a vector along the local X axis to the edge of the sphere
	glm::vec2 end = glm::vec2(std::cos(m_orientation), std::sin(m_orientation)) * m_radius;
	//Draw the circle
	aie::Gizmos::add2DCircle(m_position, m_radius, 24, m_colour);
	//Draw a line from the center of the sphere to the edge of the sphere along its local X axis.
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
}
