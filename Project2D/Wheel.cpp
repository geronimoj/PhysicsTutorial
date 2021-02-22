#include <Input.h>
#include "Wheel.h"

Wheel::Wheel(glm::vec2 position, float mass, float radius, float angularAcceleration, float angularVelocityLimit, glm::vec4 colour, unsigned int layer, float linearDrag, float angularDrag, float elasticity, float staticFriction, float kineticFriction)
	: Sphere(position, false, glm::vec2(0), mass, radius, colour, 0, layer, linearDrag, angularDrag, elasticity, staticFriction, kineticFriction), m_angularAcceleration(angularAcceleration), m_angularVelocityLimit(angularVelocityLimit)
{
}

void Wheel::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		if (m_angularVelocity < 0)
			m_angularVelocity -= m_angularAcceleration * timeStep;
		else
			m_angularVelocity -= m_angularAcceleration * timeStep * 2;
	}

	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		if (m_angularVelocity > 0)
			m_angularVelocity += m_angularAcceleration * timeStep;
		else
			m_angularVelocity += m_angularAcceleration * timeStep * 2;
	}

	Sphere::FixedUpdate(gravity, timeStep);
}
