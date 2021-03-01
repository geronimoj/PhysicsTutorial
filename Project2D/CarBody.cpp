#include "CarBody.h"

void CarBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	//Get the velocity of the wheel.
	//Apply it as a force to the equivalent point on the car
	//Set the wheels velocity to be zero
	Box::FixedUpdate(gravity, timeStep);
}
