#include <Input.h>
#include "Wheel.h"

Wheel::Wheel(glm::vec2 position, float mass, float radius, float angularAcceleration, float angularVelocityLimit, glm::vec4 colour, unsigned int layer, float linearDrag, float angularDrag, float elasticity, float staticFriction, float kineticFriction)
	: Sphere(position, false, glm::vec2(0), mass, radius, colour, 0, layer, linearDrag, angularDrag, elasticity, staticFriction, kineticFriction), m_angularAcceleration(angularAcceleration), m_angularVelocityLimit(angularVelocityLimit)
{
}

void Wheel::FixedUpdate(glm::vec2 gravity, float timeStep)
{	//Get the inputs
	aie::Input* input = aie::Input::getInstance();
	//Check for a W input
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{	//Apply regular angular acceleration if we are already rotating in the correct direction
		if (m_angularVelocity < 0)
			m_angularVelocity -= m_angularAcceleration * timeStep;
		else
			//Otherwise double the acceleration.
			m_angularVelocity -= m_angularAcceleration * timeStep * 2;
	}
	//Repeat for S input. see if statement above for comments
	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		if (m_angularVelocity > 0)
			m_angularVelocity += m_angularAcceleration * timeStep;
		else
			m_angularVelocity += m_angularAcceleration * timeStep * 2;
	}
	//Perform the spheres update to the physics part
	Sphere::FixedUpdate(gravity, timeStep);
}
