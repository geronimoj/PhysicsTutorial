#include <iostream>
#include "Rigidbody.h"
#include "PhysicsScene.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, bool isKinematic, glm::vec2 velocity, unsigned int layer, float orientation, float mass, float elasticity, float angularVelocity, float linearDrag, float angularDrag, float staticFriction, float kineticFriction)
	: PhysicsObject(shapeID, layer, elasticity, staticFriction, kineticFriction), m_position(position), m_isKinematic(isKinematic), m_velocity(velocity), m_orientation(glm::radians(orientation)), m_mass(mass), m_angularVelocity(angularVelocity), m_moment(0), m_linearDrag(linearDrag), m_angularDrag(angularDrag), m_collidedThisUpdate(false), m_collidedPreviousUpdate(false), m_isStatic(false)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_collidedPreviousUpdate = m_collidedThisUpdate;

	if (m_isKinematic)
	{
		m_velocity = glm::vec2(0, 0);
		m_angularVelocity = 0;
		return;
	}
	//Apply drag before moving
	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	//Ensure we don't end up flipping the vectors or negative numbers
	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		if (glm::length(m_velocity) < glm::length(gravity) * m_linearDrag * timeStep)
			m_velocity = glm::vec2(0, 0);

	if (glm::abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
		m_angularVelocity = 0;

	//Move the object
	m_position += m_velocity * timeStep;
	//Multiply gravity by mass to hand it in as a force. This is then accounted for in ApplyForce
	ApplyForce(gravity * m_mass * timeStep, glm::vec2(0,0));


	m_orientation += m_angularVelocity * timeStep;
	//We reset this to false after the loop to avoid gravity being counted as a collision
	m_collidedThisUpdate = false;
}

void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / GetMass();

	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / GetMoment();

	m_collidedThisUpdate = true;
}

void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal, float pen)
{
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->GetPosition() - GetPosition());
	glm::vec2 perp(normal.y, -normal.x);

	glm::vec2 lC1 = contact - m_position;
	glm::vec2 lC2 = contact - actor2->GetPosition();

	float r1 = glm::dot(lC1, -perp);
	float r2 = glm::dot(lC2, perp);
	
	float v1 = glm::dot(m_velocity, normal) - r1 * GetAngularVelocity();
	float v2 = glm::dot(actor2->GetVelocity(), normal) + r2 * actor2->GetAngularVelocity();

	if (v1 > v2)
	{
		float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

		r1 = glm::dot(lC1, -normal);
		r2 = glm::dot(lC2, normal);

		float mass1 = PhysicsScene::GetMass0(*this, r1);
		float mass2 = PhysicsScene::GetMass0(*actor2, r2);
		//Get the relative velocities of the collision points
		glm::vec2 relVel1 = GetRelativeVelocity(lC1);
		glm::vec2 relVel2 = actor2->GetRelativeVelocity(lC2);
		//Get the combined relative velocity
		glm::vec2 relVelT = relVel2 - relVel1;
		//Get the change in perpendicular velocity. This determines the max friction and the friction factor
		float perpChange = glm::dot(relVelT, perp);
		//Calculate the friction factor
		float friction = glm::abs(glm::abs(perpChange)) <= 0.0001f ? m_staticFrictionCo + actor2->GetStaticFriction() : m_kinematicFrictionCo + actor2->GetKinematicFriction();
		friction /= 2;
		//Friction
		//Calculate their acceleration into each other which is their combined acceleration along the normal
		float ac = glm::dot(relVel1 + GetMass() * PhysicsScene::GetGravity(),-normal);
		ac += glm::dot(relVel2 + actor2->GetMass() * PhysicsScene::GetGravity(), normal);
		//Calculate the normal force. The only difference is the mass of the two objects
		glm::vec2 nForce1 = -normal * (GetMass() * ac);
		glm::vec2 nForce2 = normal * (actor2->GetMass() * ac);
		//Calculate the maximum friction for both points
		float fMax1 = friction * glm::length(nForce1);
		float fMax2 = friction * glm::length(nForce2);
		//Calculate friction force
		//The force each of the objects take should be determined by their mass. The heavier the object, the less friction force is should take
		//If the mass is INT_MAX, it should take no friction force and instead, the other object should take all the friction force.
		//For equal masses, force should be equal
		//The force it takes should be determined by the difference in mass.
		//Make sure the direction of the force is not affecting the final force results
		float fForce = glm::abs(perpChange / 2);
		float force1 = 0;
		float force2 = 0;
		//The ratio cannot be above 1 so we need to make sure the larger mass is on the right
		if (GetMass() < actor2->GetMass())
		{	
			float ratio = GetMass() / actor2->GetMass();
			force1 = fForce * (1 + (1 - ratio));
			force2 = fForce * ratio;
		}
		else
		{
			float ratio = actor2->GetMass() / GetMass();
			force2 = fForce * (1 + (1 - ratio));
			force1 = fForce * ratio;
		}
		//Clamp the force if necessary
		if (glm::abs(force1) > fMax1)
			force1 = force1 < 0 ? -fMax1 : fMax1;
		if (glm::abs(force2) > fMax2)
			force2 = force2 < 0 ? -fMax2 : fMax2;
		//Give it mass so its a force now
		force1 *= mass1;
		force2 *= mass2;

		float dot = glm::dot(relVelT, perp);

		//If dot > 0, that means the relative velocity and perp vector and parallel which means perp needs to be inverted
		//If dot < 0, the relative velocity and perp vector are already pointing in opposite directions so perp is already correct
		//relVelT is a vector from actor1 to actor2's velocity
		glm::vec2 dir = glm::vec2(0);
		if (dot > 0)
			dir = -perp;
		else if (dot < 0)
			dir = perp;

		//Apply velocity of CoM as force assuming the point of contact is the CoM
		//Reguardless as to whether the point of contact is stationary, it cannot go down and thus, will act as the pivot point instead of the CoM
		//1. Get velocity of CoM as force
		glm::vec2 coMForce = GetVelocity() * GetMass();
		//2. Get a sphere assuming its CoM is the contact point
		//Moment of a point mass is: mr^2
		r1 = glm::length(lC1);
		r2 = glm::length(lC2);
		float pointMoment = GetMass() * (r1 * r1);
		//3. Apply the force to the sphere at an equal point to where the original CoM was. The CoM of this sphere is not allowed to gain velocity
		float torque = coMForce.y * -lC1.x - coMForce.x * -lC1.y;
		//If we don't have any torque, there is no point in continuing
		if (torque != 0)
		{	//4. Calculate the resultant relative velocity and set the velocity of the CoM to be that.
			//Get the final velocity
			glm::vec2 finalVel = (torque / pointMoment) * glm::vec2(lC1.y, -lC1.x);
			//Get the difference between current and final
			finalVel -= GetVelocity();
			//Scale final by friction factor
			finalVel *= friction;
			//Apply the dif to velocity
			SetVelocity(GetVelocity() + finalVel);
		}
		//Repeat for actor2
		coMForce = actor2->GetVelocity() * actor2->GetMass();
		pointMoment = actor2->GetMass() * (r2 * r2);
		torque = coMForce.y * -lC2.x - coMForce.x * -lC2.y;

		if (torque != 0)
		{	//Get the final velocity
			glm::vec2 finalVel = (torque / pointMoment) * glm::vec2(lC2.y, -lC2.x);
			//Get the difference between current and final
			finalVel -= actor2->GetVelocity();
			//Scale final by friction factor
			finalVel *= friction;
			//Apply the dif to velocity
			actor2->SetVelocity(actor2->GetVelocity() + finalVel);
		}

		//Calculate the direction the force should be applied
		ApplyForce(force1 * -dir, lC1);
		//Apply their force in the opposite direction
		actor2->ApplyForce(force2 * dir, lC2);

		//Elasticity
		r1 = glm::dot(lC1, -perp);
		r2 = glm::dot(lC2, perp);
		//Get the effective mass
		mass1 = PhysicsScene::GetMass0(*this, r1);
		mass2 = PhysicsScene::GetMass0(*actor2, r2);

		glm::vec2 eForce = (1 + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;
		//Accout for the velocity that friction may or may not have accounted for due to inconsistencies.
		glm::vec2 velocityChange = GetRelativeVelocity(lC1) - relVel1;
		float reboundAccountedFor = glm::dot(velocityChange, normal);

		force1 = glm::length(eForce / mass1) + reboundAccountedFor;
		force1 *= mass1;

		//Repeat for second body
		velocityChange = actor2->GetRelativeVelocity(lC2) - relVel2;
		reboundAccountedFor = glm::dot(velocityChange, normal);

		force2 = glm::length(eForce / mass2) - reboundAccountedFor;
		force2 *= mass2;
		//Apply elastic force
		ApplyForce(-glm::normalize(eForce) * force1, lC1);
		actor2->ApplyForce(glm::normalize(eForce) * force2, lC2);

		if (pen > 0)
			PhysicsScene::ApplyContactForces(this, actor2, normal, pen);
	}
}

float Rigidbody::GetPotentialEnergy()
{	//If we are kinematic, we don't have potential energy
	return m_isKinematic ? 0 : GetMass() * glm::length(PhysicsScene::GetGravity()) * m_position.y;
}
