#pragma once
#include "Rigidbody.h"
/// <summary>
/// Contains information for simulating drawing and creating a sphere.
/// </summary>
class Sphere : public Rigidbody
{
public:
	/// <summary>
	/// Creates a sphere
	/// </summary>
	/// <param name="position">The position of the sphere</param>
	/// <param name="isKinematic">The kinematic state of the sphere. Determines if the sphere should be simulated by and interact with physics</param>
	/// <param name="velocity">The inital velocity of the sphere</param>
	/// <param name="mass">The mass of the sphere</param>
	/// <param name="radius">The radius of the sphere</param>
	/// <param name="colour">The colour of the sphere in RGBA format</param>
	/// <param name="angularVelocity">The initial angular velocity of the sphere in radians per second. Defaults to 0</param>
	/// <param name="layer">The inital layer of the sphere. Defaults to layer 0</param>
	/// <param name="linearDrag">The linear drag coefficient. Defaults to 0</param>
	/// <param name="angularDrag">The angular drag coefficient. Defaults to 0</param>
	/// <param name="elasticity">The elasticity factor. Set to 1 for no gain or loss of energy. Defaults to 1.</param>
	/// <param name="staticFriction">The static friction coefficient. Defaults to 1</param>
	/// <param name="kineticFriction">The kinetic friction coefficient. Defaults to 1</param>
	Sphere(glm::vec2 position, bool isKinematic, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, float angularVelocity = 0, unsigned int layer = 0, float linearDrag = 0, float angularDrag = 0, float elasticity = 1, float staticFriction = 1, float kineticFriction = 1);
	~Sphere();
	/// <summary>
	/// Draws the sphere with a line to indicate orientation.
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// Returns the radius
	/// </summary>
	/// <returns>Returns the radius</returns>
	float GetRadius() const { return m_radius; }
	/// <summary>
	/// Returns the colour in RGBA format
	/// </summary>
	/// <returns>Returns the colour in RGBA format as a vector4</returns>
	glm::vec4 GetColour() const { return m_colour; }

protected:
	/// <summary>
	/// The radius of the sphere
	/// </summary>
	float m_radius;
	/// <summary>
	/// The colour of the sphere
	/// </summary>
	glm::vec4 m_colour;
};

