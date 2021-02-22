#pragma once
#include "Sphere.h"
/// <summary>
/// A user controller sphere. The user can control the angular velocity of the sphere
/// </summary>
class Wheel : public Sphere
{
public:
	/// <summary>
	/// Creates a wheel
	/// </summary>
	/// <param name="position">The initial position of the wheel</param>
	/// <param name="mass">The mass of the wheel</param>
	/// <param name="radius">The radius of the wheel</param>
	/// <param name="angularAcceleration">The acceleration applied to the angular velocity when an input is made</param>
	/// <param name="angularVelocityLimit">The maximum angular velocity</param>
	/// <param name="colour">The colour of the wheel in RGBA format</param>
	/// <param name="layer">The layer the wheel exists on</param>
	/// <param name="linearDrag">The linear drag coefficient. Defaults to 0</param>
	/// <param name="angularDrag">The angular drag coefficient. Defaults to 0</param>
	/// <param name="elasticity">The elasticity factor. Set to 1 for no loss or gain of energy. Defaults to 1</param>
	/// <param name="staticFriction">The static friction coefficient. Defaults to 1</param>
	/// <param name="kineticFriction">The kinetic friction coefficient. Defaults to 1</param>
	Wheel(glm::vec2 position, float mass, float radius, float angularAcceleration, float angularVelocityLimit, glm::vec4 colour, unsigned int layer, float linearDrag = 0, float angularDrag = 0, float elasticity = 1, float staticFriction = 1, float kineticFriction = 1);
	~Wheel();
	/// <summary>
	/// Applies angular acceleration and clamps velocity
	/// </summary>
	/// <param name="gravity">The direction and magnitude of gravity</param>
	/// <param name="timeStep">The time passed for this physics simulation</param>
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

protected:
	/// <summary>
	/// How quickly the angular velocity changes
	/// </summary>
	float m_angularAcceleration;
	/// <summary>
	/// The maximum angular velocity
	/// </summary>
	float m_angularVelocityLimit;
};

