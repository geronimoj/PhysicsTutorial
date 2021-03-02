#include "CarBody.h"
#include "Box.h"

CarBody::CarBody(PhysicsScene* scene, unsigned int layer, glm::vec2 position, glm::vec2 bodyExtents, glm::vec2 frontWheel, glm::vec2 backWheel, float mass, glm::vec4 bodyColour, glm::vec4 wheelColour, float wheelMass, float wheelRadius, float wheelAngAccel, float wheelAngVelLimit, float linearDrag, float angularDrag, float wheelElasticity)
	: Box(position, false, glm::vec2(0), mass, bodyColour, bodyExtents, 0, 0, layer, linearDrag, angularDrag, 0.3f, 0.5f, 0.5f), m_frontOrigin(frontWheel), m_backOrigin(backWheel)
{	//Since we add the wheels to the scenes actors, we don't have to deal with cleaning them up :D
	m_backWheel = new Wheel(ToWorld(m_backOrigin), wheelMass, wheelRadius, wheelAngAccel, wheelAngVelLimit, wheelColour, layer, 0, angularDrag / 2, wheelElasticity, 1, 1);
	m_frontWheel = new Wheel(ToWorld(m_frontOrigin), wheelMass, wheelRadius, wheelAngAccel, wheelAngVelLimit, wheelColour, layer, 0, angularDrag / 2, wheelElasticity, 1, 1);

	scene->AddActor(m_backWheel);
	scene->AddActor(m_frontWheel);
}

void CarBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	glm::vec2 frontRelVel = GetRelativeVelocity(m_frontOrigin);
	glm::vec2 backRelVel = GetRelativeVelocity(m_backOrigin);
	//Get the velocity of the wheel.
	//Get the relative velocity of the point the wheel is attached to
	glm::vec2 relVel = m_backWheel->GetVelocity();
	//Get the change in velocity from its velocity to the relative velocity
	relVel -= backRelVel;
	//Apply the change to the point of connection of the wheel and the car body as a force
	float mass0 = PhysicsScene::GetMass0(*this, glm::dot(m_backOrigin, glm::normalize(glm::vec2(relVel.y, -relVel.x))));
	//Apply the force
	ApplyForce(relVel * mass0, m_backOrigin);

	//Repeat for front wheel
	relVel = m_frontWheel->GetVelocity();
	relVel -= frontRelVel;
	mass0 = PhysicsScene::GetMass0(*this, glm::dot(m_frontOrigin, glm::normalize(glm::vec2(relVel.y, -relVel.x))));

	ApplyForce(relVel * mass0, m_frontOrigin);

	bool fixForBody = m_frontWheel->CollidedThisUpdate() || m_backWheel->CollidedThisUpdate();
	//Do other stuff
	Box::FixedUpdate(gravity, timeStep);

	//Reset the velocity of the two wheels
	m_backWheel->SetVelocity(GetRelativeVelocity(m_backOrigin));
	m_frontWheel->SetVelocity(GetRelativeVelocity(m_frontOrigin));

	//If its the box that's too close
	if (fixForBody)
		m_position = m_frontWheel->GetPosition() + m_localX * -m_frontOrigin.x + m_localY * -m_frontOrigin.y;
	//Reset the positions of the wheels to be the correct point
	m_backWheel->SetPosition(ToWorld(m_backOrigin));
	m_frontWheel->SetPosition(ToWorld(m_frontOrigin));
}
