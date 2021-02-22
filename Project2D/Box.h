#pragma once
#include "Rigidbody.h"
/// <summary>
/// Contains all necessary information for creating and simulating Cubes and Rectangles.
/// </summary>
class Box :	public Rigidbody
{
public:
	/// <summary>
	/// Creates a box
	/// </summary>
	/// <param name="position">The position of the origin of the box</param>
	/// <param name="isKinematic">Should the box be simulated by the physics engine</param>
	/// <param name="velocity">The inital velocity of the boxes Center of Mass in units per second</param>
	/// <param name="mass">The mass of the Center of Mass</param>
	/// <param name="colour">The colour of the box in RGBA format</param>
	/// <param name="extents">The half extents of the box</param>
	/// <param name="orientation">The inital orientation of the box in degrees</param>
	/// <param name="angularVelocity">The inital angular velocity of the box in radians per second or per second</param>
	/// <param name="layer">The layer of the box. Determines what it can collide with. Defaults to layer 0</param>
	/// <param name="linearDrag">The linear drag of the box. Defaults to 0</param>
	/// <param name="angularDrag">The angular drag of the box. Defaults to 0</param>
	/// <param name="elasticity">The elasticity of the box. Determines how much energy is gained or lost from a collision with this physics object. Defaults to 1 (no energy loss or gain)</param>
	/// <param name="staticFriction">The static friction coefficient. The friction factor applied on a stationary object. Defaults to 1. Set to 0 for frictionless</param>
	/// <param name="kinematicFriction">The kinetic friction coefficient. The friction factor applied on a moving object. Defaults to 1. Set to 0 for frictionless</param>
	Box(glm::vec2 position, bool isKinematic, glm::vec2 velocity, float mass, glm::vec4 colour, glm::vec2 extents, float orientation, float angularVelocity, unsigned int layer = 0, float linearDrag = 0, float angularDrag = 0, float elasticity = 1, float staticFriction = 1, float kineticFriction = 1);
	~Box();
	/// <summary>
	/// Rotates, transforms and applys drag to the box. Also re-calculates local X and Y axis.
	/// </summary>
	/// <param name="gravity">The direction and magnitude of gravity.</param>
	/// <param name="timeStep">The time that has passed for this physics update.</param>
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	/// <summary>
	/// Draws the box
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// Determines if this box is intersecting with the given box and returns information about the contact point.
	/// </summary>
	/// <param name="box">The box to compare to</param>
	/// <param name="contact">The point of contact. Does not override the original value, it instead adds to the value.</param>
	/// <param name="numContacts">The number of contacts between the boxes. Does not override the original value, it increments the value instead.</param>
	/// <param name="pen">A reference to a float for writing purposes. How far the contact point penetrates the box.</param>
	/// <param name="edgeNormal">A reference to a glm::vec2 for writing purposes. The normal of the edge the collision occured on.</param>
	/// <returns>Returns true if there is a collision and writes to the parameters information about the collision.</returns>
	bool CheckBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);
	/// <summary>
	/// Transforms a local position/offset into world co-ordinates.
	/// Accounts for box rotation.
	/// </summary>
	/// <param name="offset">The local position or offset from the center of the box.</param>
	/// <returns>Returns the equivalent position of "offset" in world co-ordinates.</returns>
	virtual glm::vec2 ToWorld(glm::vec2 offset) { return GetPosition() + GetLocalX() * offset.x + GetLocalY() * offset.y; }
	/// <summary>
	/// Returns the half extents of the box.
	/// </summary>
	/// <returns>Returns the half extents of the box as a vector2.
	/// The x represents half the width, the y represents half the height. (Half extents) </returns>
	glm::vec2 GetExtents() const { return m_extents; }
	/// <summary>
	/// Returns the local X axis.
	/// </summary>
	/// <returns>Returns the local X axis as a vector2.</returns>
	glm::vec2 GetLocalX() const { return m_localX; }
	/// <summary>
	/// Returns the local Y axis.
	/// </summary>
	/// <returns>Returns the local Y axis as a vector2.</returns>
	glm::vec2 GetLocalY() const { return m_localY; }
	/// <summary>
	/// Returns the width.
	/// </summary>
	/// <returns>The length of the box along the local X axis.</returns>
	float GetWidth() const { return m_extents.x * 2; }
	/// <summary>
	/// Returns the height.
	/// </summary>
	/// <returns>The length of the box along the local Y axis.</returns>
	float GetHeight() const { return m_extents.y * 2; }

protected:
	/// <summary>
	/// The half extents
	/// </summary>
	glm::vec2 m_extents;
	/// <summary>
	/// The colour of the box in RBGA
	/// </summary>
	glm::vec4 m_colour;
	/// <summary>
	/// The local X axis
	/// </summary>
	glm::vec2 m_localX;
	/// <summary>
	/// The local Y axis
	/// </summary>
	glm::vec2 m_localY;
};

