#include <Gizmos.h>
#include <iostream>
#include "Plane.h"
#include "PhysicsScene.h"
#define INFERIOR
#define DEBUG

Plane::Plane(glm::vec2 normal, float distance, unsigned int layer, glm::vec4 colour, float elasticity, float staticFriction, float kineticFriction)
	: PhysicsObject(ShapeType::PLANE, elasticity, staticFriction, kineticFriction, layer), m_normal(glm::normalize(normal)), m_distanceToOrigin(distance), m_colour(colour)
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
{	//Store the local contact point
	glm::vec2 localContact = contact - actor2->GetPosition();
	//Store the perpendicular vector
	glm::vec2 perp(m_normal.y, -m_normal.x);
	//Store the relative velocity
	glm::vec2 relativeVelocity = actor2->GetRelativeVelocity(localContact);
	//Calcualate initial energy
	float kePre = actor2->getEnergy();
	float lKePre = actor2->GetLinearEnergy();
	float aKePre = actor2->GetRotationalKineticEnergy();
	//Calculate friction and elasticity coefficients
	float friction = glm::abs(glm::dot(relativeVelocity, perp)) <= 0.0001f ? m_staticFrictionCo + actor2->GetStaticFriction() : m_kinematicFrictionCo + actor2->GetKinematicFriction();
	friction /= 2;
	float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;
	//Setup storage locations for variables. This is primarily from when I was moving code around a lot
	float r = 0;
	float mass0 = 0;
	float lossToFriction = 0;
	float lossToElasticity = 0;
	glm::vec2 force = glm::vec2(0);
	//Friction
	//Get the acceleration over this time skip. Since this is a plane, the expectedVelocity (inital relative velocity)
	//Would be equal to the acceleration the plane puts onto it to stop it from intersecting it
	float accel = glm::dot(relativeVelocity + actor2->GetMass() * PhysicsScene::GetGravity(), -m_normal);
	//Calculate the velocity along the plane and normal force
	glm::vec2 velocityAlongPlane = perp * glm::dot(relativeVelocity, perp);
	glm::vec2 normalForce = -m_normal * (actor2->GetMass() * accel);
	//Stored as a float because friction force will always be opposed to objects motion
	float frictionMax = friction * glm::length(normalForce);
	//Calculate mass0 for friction
	r = glm::dot(localContact, m_normal);
	mass0 = PhysicsScene::GetMass0(*actor2, r);
	//Get the friction force. We want the smaller value
	force = frictionMax < glm::length(velocityAlongPlane) ?
		//If friction is smaller use frictionForce
		frictionMax * -glm::normalize(velocityAlongPlane) * mass0
		//If velocity is smaller use velocity
		: -velocityAlongPlane * mass0;

	//Calculate torque to calculate the energy loss due to friction
	float torque = force.y * localContact.x - localContact.y * force.x;
	//From torque, we can calculate the angularAcceleration caused by the torque
	//R - Torque
	//I - Moment
	//a - Angular Acceleration
	//R = Ia
	//a = R / I
	float angularAcceleration = torque / actor2->GetMoment();
	//Now we know the final angular velocity, we can calculate the rotational kinetic energy at this moment in time.
	float expectedAngularVelocity = actor2->GetAngularVelocity() + angularAcceleration;
	//We have now calculated the final energy from friction but we need the change
	lossToFriction = 0.5f * actor2->GetMoment() * (expectedAngularVelocity * expectedAngularVelocity);
	//aKePre is the angular kinetic energy before friction. It was re-calculated after elasticity.
	//Now we know the change in rotational kinetic energy. I should probably rename those functions.
	lossToFriction = aKePre - lossToFriction;
	//Calculate the change in linear velocity from this force
	glm::vec2 linearVelocity = force / actor2->GetMass();
	//Calculate the final linear velocity and thus, final linear kinetic energy. And then get the change in energy and done
	linearVelocity = actor2->GetVelocity() + linearVelocity;
	lossToFriction += lKePre - (0.5f * actor2->GetMass() * glm::dot(linearVelocity, linearVelocity));

	//Apply velocity of CoM as force assuming the point of contact is the CoM
	//Reguardless as to whether the point of contact is stationary, it cannot go down and thus, will act as the pivot point instead of the CoM
	//1. Get velocity of CoM as force
	glm::vec2 coMForce = actor2->GetVelocity() * actor2->GetMass();
	//2. Get a sphere assuming its CoM is the contact point
	//Moment of a point mass is: mr^2
	r = glm::length(localContact);
	float pointMoment = actor2->GetMass() * (r * r);
	//3. Apply the force to the sphere at an equal point to where the original CoM was. The CoM of this sphere is not allowed to gain velocity
	torque = coMForce.y * -localContact.x - coMForce.x * -localContact.y;
	//If we don't have any torque, there is no point in continuing
	if (torque != 0)
	{
		//4. Calculate the resultant relative velocity and set the velocity of the CoM to be that.
		angularAcceleration = torque / pointMoment;
		linearVelocity = angularAcceleration * glm::vec2(localContact.y, -localContact.x);
		actor2->SetVelocity(linearVelocity);
	}
	//Apply the force now we have the force of the CoM
	actor2->ApplyForce(force, localContact);
	//Calculate the change in vertical force that we've already applied
	glm::vec2 velocityChange = actor2->GetRelativeVelocity(localContact);
	velocityChange -= relativeVelocity;
	//Get only the vertical component
	float reboundAccountedFor = glm::dot(velocityChange, m_normal);

	//Elasticity
	float velocityIntoPlane = glm::dot(relativeVelocity, m_normal);
	//Elasticity
	//Get the effective mass of the contact point
	r = glm::dot(localContact, perp);
	mass0 = PhysicsScene::GetMass0(*actor2, r);
	//Calculate the magnitude of force to apply
	float j = -(1 + elasticity) * velocityIntoPlane * mass0;
	//Give the force a direction
	force = m_normal * (j - (reboundAccountedFor * mass0));

	//Apply the vertical bounce force off of the plane
	actor2->ApplyForce(force, localContact);
	//Since we know, from previous tests, that the elasticity part does not gain or lose energy (except for floating point errors)
	//We can just cheat to calculate the loss due to elasticity.
	lossToElasticity = kePre - actor2->getEnergy() - lossToFriction;

	float kePost = actor2->getEnergy() + lossToFriction + lossToElasticity;
	float delta = kePost - kePre;
	if (glm::abs(delta) > glm::abs(kePost) * 0.01f)
		std::cout << "Kinetic Energy discrepancy greather than 1%." << std::endl << "Linear Energy Dif:" << actor2->GetLinearEnergy() - lKePre << "	Angular Energy Dif: " << actor2->GetRotationalKineticEnergy() - aKePre << std::endl;

	float pen = glm::dot(contact, m_normal) - m_distanceToOrigin;
	PhysicsScene::ApplyContactForces(actor2, nullptr, m_normal, pen);
}