#pragma once

#include "PhysicsObject.h"
/// <summary>
/// Contains the information for creating and simulating a rigidbody.
/// </summary>
class Rigidbody : public PhysicsObject
{
public:
	/// <summary>
	/// Creates a rigid body
	/// </summary>
	/// <param name="shapeID">The ID of the shape</param>
	/// <param name="position">The position of the body</param>
	/// <param name="isKinematic">The kinematic state of the body. Set to true if you don't want the body to be simulated by physics</param>
	/// <param name="velocity">The inital velocity of the object</param>
	/// <param name="layer">The layer the body exists on</param>
	/// <param name="orientation">The orientation, in degrees of the body</param>
	/// <param name="mass">The mass of the body</param>
	/// <param name="elasticity">The elasticity of the body. Set to 1 for no energy loss or gain</param>
	/// <param name="angularVelocity">The inital angular velocity</param>
	/// <param name="linearDrag">The linear drag for this body</param>
	/// <param name="angularDrag">The angular drag for this body</param>
	/// <param name="staticFriction">The static friction coefficient for this body</param>
	/// <param name="kineticFriction">The kinetic friction coefficient for this body</param>
	Rigidbody(ShapeType shapeID, glm::vec2 position, bool isKinematic, glm::vec2 velocity, unsigned int layer, float orientation, float mass, float elasticity, float angularVelocity, float linearDrag, float angularDrag, float staticFriction, float kineticFriction);
	~Rigidbody();
	/// <summary>
	/// Simulates the movement, rotation and drag of the rigidbody
	/// </summary>
	/// <param name="gravity">The direction and magnitude of gravity</param>
	/// <param name="timeStep">The time that has passed for this time step</param>
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	/// <summary>
	/// Applies a force to the rigidbody at a given local point
	/// </summary>
	/// <param name="force">The force with direction to apply</param>
	/// <param name="pos">The local point to apply the force to</param>
	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	/// <summary>
	/// Resolves the collision between this rigidbody and another rigidbody
	/// </summary>
	/// <param name="actor2">The rigidbody to resolve with</param>
	/// <param name="contact">The contact point</param>
	/// <param name="collisionNormal">The collision normal. If nullptr, collision normal will be calculated as the vector between the two objects origins</param>
	/// <param name="pen">How much the two objects are penetrating each other. Defaults to 0</param>
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2 * collisionNormal = nullptr, float pen = 0);
	/// <summary>
	/// Sets the position if the object is not static and kinematic
	/// </summary>
	/// <param name="position">The position to be set to</param>
	void SetPosition(glm::vec2 position) { if (!(m_isKinematic && m_isStatic)) m_position = position; }
	/// <summary>
	/// Sets the kinematic state of the rigidbody
	/// </summary>
	/// <param name="state">The state to be set to</param>
	void SetKinematic(bool state) { m_isKinematic = state; }
	/// <summary>
	/// Sets the angular velocity
	/// </summary>
	/// <param name="velocity">The angular velocity to be set to</param>
	void SetAngularVelocity(float velocity) { m_angularVelocity = velocity; }
	/// <summary>
	/// Sets the velocity
	/// </summary>
	/// <param name="velocity">The velocity to be set to</param>
	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	/// <summary>
	/// Sets the static state of the body. Will only affect anything on kinematic bodies
	/// </summary>
	/// <param name="state">The state to set isStatic to</param>
	void SetStatic(bool state) { m_isStatic = state; }
	/// <summary>
	/// Returns the velocity of a point on the object
	/// </summary>
	/// <param name="localContact">The local point to get the velocity at</param>
	/// <returns>Returns the velocity of a particle at that local point as a vector2</returns>
	glm::vec2 GetRelativeVelocity(glm::vec2 localContact) { return m_velocity + m_angularVelocity * glm::vec2(-localContact.y, localContact.x); }
	/// <summary>
	/// Returns the position
	/// </summary>
	/// <returns>Returns the position</returns>
	glm::vec2 GetPosition() const { return m_position; }
	/// <summary>
	/// Returns the velocity
	/// </summary>
	/// <returns>Returns the velocity as a vector2</returns>
	glm::vec2 GetVelocity() const { return m_velocity; }
	/// <summary>
	/// Converts an offset in local space to global space
	/// </summary>
	/// <param name="offset">The local offset/position to convert</param>
	/// <returns>Returns the equivalent global position of offset as a vector2</returns>
	virtual glm::vec2 ToWorld(glm::vec2 offset) { return GetPosition() + offset; }
	/// <summary>
	/// Returns the orientation
	/// </summary>
	/// <returns>Returns the orientation in radians</returns>
	float GetOrientation() const { return m_orientation; }
	/// <summary>
	/// Returns the mass
	/// </summary>
	/// <returns>Returns the mass</returns>
	float GetMass() const { return m_isKinematic ? INT_MAX : m_mass; }
	/// <summary>
	/// Returns the moment
	/// </summary>
	/// <returns>Returns the moment</returns>
	float GetMoment() const { return m_isKinematic ? INT_MAX : m_moment; }
	/// <summary>
	/// Returns the angular velocity
	/// </summary>
	/// <returns>Returns the angular velocity in radians per second</returns>
	float GetAngularVelocity() const { return m_angularVelocity; }
	/// <summary>
	/// Returns the kinematic state
	/// </summary>
	/// <returns>Returns true if the rigidbody is kinematic</returns>
	bool IsKinematic() { return m_isKinematic; }
	/// <summary>
	/// Returns if we collided with something this update. This is determined if ApplyForce was called
	/// </summary>
	/// <returns>Returns true if something was collided with</returns>
	bool CollidedThisUpdate() { return m_collidedThisUpdate || m_collidedPreviousUpdate; }
	/// <summary>
	/// Returns the total energy of the physics object
	/// </summary>
	/// <returns>
	/// Returns the sum of the lienar, rotational and potential energy</returns>
	virtual float getEnergy() {
		return GetLinearEnergy() + GetRotationalKineticEnergy() + GetPotentialEnergy();
	}
	/// <summary>
	/// Returns the linear kinetic energy
	/// </summary>
	/// <returns>Returns the linear kinetic energy</returns>
	float GetLinearEnergy() { return 0.5f * GetMass() * glm::dot(m_velocity, m_velocity); }
	/// <summary>
	/// Returns the rotational kinetic energy
	/// </summary>
	/// <returns>Returns the rotational kinetic energy</returns>
	float GetRotationalKineticEnergy() { return 0.5f * GetMoment() * (m_angularVelocity * m_angularVelocity); }
	/// <summary>
	/// Returns to potential energy
	/// </summary>
	/// <returns>Returns the potential energy</returns>
	float GetPotentialEnergy();

protected:
	/// <summary>
	/// The position of the object
	/// </summary>
	glm::vec2 m_position;
	/// <summary>
	/// The velocity of the object
	/// </summary>
	glm::vec2 m_velocity;
	/// <summary>
	/// The angular velocity of the object
	/// </summary>
	float m_angularVelocity;
	/// <summary>
	/// The mass of the Center of Mass
	/// </summary>
	float m_mass;
	/// <summary>
	/// The orientation in radians of the object
	/// </summary>
	float m_orientation;
	/// <summary>
	/// The moment. Calculated in constructor
	/// </summary>
	float m_moment;
	/// <summary>
	/// The linear drag
	/// </summary>
	float m_linearDrag;
	/// <summary>
	/// The angular drag
	/// </summary>
	float m_angularDrag;
	/// <summary>
	/// Should the Physics Object be simulated by the physics
	/// </summary>
	bool m_isKinematic;
	/// <summary>
	/// Determines if the object can be moved. isKinematic must also be true for this to come into affect
	/// </summary>
	bool m_isStatic;
	/// <summary>
	/// Stores if this body collided with something this update.
	/// </summary>
	bool m_collidedThisUpdate;
private:
	/// <summary>
	/// Stores if this body collided with something the previous update
	/// </summary>
	bool m_collidedPreviousUpdate;
};

