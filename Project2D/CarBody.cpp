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
	//Determine if we need to teleport the body of the car or if the wheels are the ones that should be teleported
	//If the body collided this update, don't fix the body to avoid clipping issues
	bool fixForBody = m_frontWheel->CollidedThisUpdate() || m_backWheel->CollidedThisUpdate();
	bool bodyCollided = CollidedThisUpdate();
	//Calculate the change in velocity from the expected velocity to their actual velocity
	//This determines how much force needs to be applied to the car
	glm::vec2 relVel1 = m_backWheel->GetVelocity();
	relVel1 -= GetRelativeVelocity(m_backOrigin);

	//Repeat for front wheel
	glm::vec2 relVel2 = m_frontWheel->GetVelocity();
	relVel2 -= GetRelativeVelocity(m_frontOrigin);
	//Calculate the point that the force needs to be applied to. This is a ratio between the two wheels equal to the ratio between each of their forces
	glm::vec2 pointToApply = glm::length(relVel1) * m_backOrigin + glm::length(relVel2) * m_frontOrigin;
	float totalApplied = glm::length(relVel1) + glm::length(relVel2);
	pointToApply /= totalApplied;

	//Get the average relative velocity that we want to apply to the calculated point
	relVel1 += relVel2;
	relVel1 /= 2;
	//Make sure we have relative velocity to apply. Necessary because of glm::normalize
	if (relVel1 != glm::vec2(0, 0))
	{
		float mass0 = PhysicsScene::GetMass0(*this, glm::dot(m_frontOrigin, glm::normalize(glm::vec2(relVel1.y, -relVel1.x))));
		//Apply the force
		ApplyForce(relVel1 * mass0, pointToApply);
		//Undo our changes to colliderThisUpdate
		m_collidedThisUpdate = bodyCollided;
	}
	//Do other stuff
	Box::FixedUpdate(gravity, timeStep);

	//Reset the velocity of the two wheels
	m_backWheel->SetVelocity(GetRelativeVelocity(m_backOrigin));
	m_frontWheel->SetVelocity(GetRelativeVelocity(m_frontOrigin));

	//If its the box that's too close to the wheels. Teleport it away but only if it didn't collide with anything
	if (fixForBody)
	{
		glm::vec2 averagePoint = (m_frontOrigin + m_backOrigin) / 2.0f;
		glm::vec2 averageWorldPoint = (m_frontWheel->GetPosition() + m_backWheel->GetPosition()) / 2.0f;
		m_position = averageWorldPoint + m_localX * -averagePoint.x + m_localY * -averagePoint.y;
		//We still redo the wheels but only if they, themself are not colliding with anything
		if (!(bodyCollided && m_backWheel->CollidedThisUpdate()))
			m_backWheel->SetPosition(ToWorld(m_backOrigin));
		if (!(bodyCollided && m_frontWheel->CollidedThisUpdate()))
			m_frontWheel->SetPosition(ToWorld(m_frontOrigin));
	}
	else
	{
		//Reset the positions of the wheels to be the correct point but only if they didn't collide with anything
		m_backWheel->SetPosition(ToWorld(m_backOrigin));
		m_frontWheel->SetPosition(ToWorld(m_frontOrigin));
	}
}
