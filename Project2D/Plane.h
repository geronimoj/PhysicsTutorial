#pragma once

#include "PhysicsObject.h"
#include "Rigidbody.h"
/// <summary>
/// Contains all necessary information for creating and simulating static infinite planes
/// </summary>
class Plane : public PhysicsObject
{
public:
	/// <summary>
	/// Creates a plane
	/// </summary>
	/// <param name="normal">The normal of the plane</param>
	/// <param name="distance">The distance from the origin of the world the plane exists</param>
	/// <param name="layer">The layer this plane exists on</param>
	/// <param name="colour">The colour of the plane in RGBA format</param>
	/// <param name="elasticity">The elasticity factor for the plane. Defaults to 1 for no gain or loss of energy.</param>
	/// <param name="staticFriction">The static friction coefficient. Defaults to 1</param>
	/// <param name="kinematicFriction">The kinetic friction coefficient. Defaults to 1</param>
	Plane(glm::vec2 normal, float distance, unsigned int layer, glm::vec4 colour, float elasticity = 1, float staticFriction = 1, float kineticFriction = 1);
	~Plane();
	/// <summary>
	/// Does nothing. Only exists because fixedUpdate is pure virtual
	/// </summary>
	/// <param name="gravity">The direction and magnitude of gravity</param>
	/// <param name="timeStep">The time step for this physics update</param>
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) {}
	/// <summary>
	/// Draws the plane
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// Does nothing
	/// </summary>
	virtual void ResetPosition() {}
	/// <summary>
	/// Resolves collisions with other rigidbodies onto this plane
	/// </summary>
	/// <param name="actor2">The actor to resolve the collision with</param>
	/// <param name="contact">The contact point where the collision took place</param>
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact);
	/// <summary>
	/// Returns the normal of the plane
	/// </summary>
	/// <returns>Returns the normal of the plane as a vector2</returns>
	glm::vec2 GetNormal() const { return m_normal; }
	/// <summary>
	/// Returns the distance from the origin of the world
	/// </summary>
	/// <returns>Returns the distance from the origin of the world</returns>
	float GetDistance() const { return m_distanceToOrigin; }
	/// <summary>
	/// Returns the colour of the plane in RGBA
	/// </summary>
	/// <returns>Returns the colour of the plane as a vector4 in RGBA formula</returns>
	glm::vec4 GetColour() const { return m_colour; }

protected:
	/// <summary>
	/// The normal of the plane
	/// </summary>
	glm::vec2 m_normal;
	/// <summary>
	/// The colour of the plane
	/// </summary>
	glm::vec4 m_colour;
	/// <summary>
	/// The distance, along the normal, from the origin of the world
	/// </summary>
	float m_distanceToOrigin;
};

