#include <Gizmos.h>
#include "Sphere.h"

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : Rigidbody(ShapeType::SPHERE, position, velocity, 0, mass, 0), m_radius(radius), m_colour(colour)
{
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

void Sphere::Draw()
{
	glm::vec2 end = glm::vec2(std::cos(m_orientation), std::sin(m_orientation)) * m_radius;
	aie::Gizmos::add2DCircle(m_position, m_radius, 24, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
}
