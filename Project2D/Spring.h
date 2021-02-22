#pragma once
#include "Rigidbody.h"
/// <summary>
/// Contains the information for creating and simulating a sphere
/// </summary>
class Spring : public PhysicsObject
{
public:
	/// <summary>
	/// Creates a spring
	/// </summary>
	/// <param name="body1">The first body</param>
	/// <param name="body2">The second body</param>
	/// <param name="restLength">The length of the spring</param>
	/// <param name="colour">The colour of the spring</param>
	/// <param name="springCoefficient">The springs strength coefficient</param>
	/// <param name="damping">The damping factor. Defaults to 0.1f</param>
	/// <param name="body1Contact">The point of connection to body1 in local co-ordinates. Defaults to body1 Center of Mass</param>
	/// <param name="body2Contact">The point of connection to body2 in local co-ordinates. Defaults to body2 Center of Mass</param>
	Spring(Rigidbody* body1, Rigidbody* body2, float restLength, glm::vec4 colour,
		float springCoefficient, float damping = 0.1f, 
		glm::vec2 body1Contact = glm::vec2(0,0), glm::vec2 body2Contact = glm::vec2(0,0));
	/// <summary>
	/// Create a spring with automatically calculated length and contact points at center of mass
	/// </summary>
	/// <param name="body1">The first body</param>
	/// <param name="body2">The second body</param>
	/// <param name="damping">The damping factor</param>
	/// <param name="springCoefficient">The springs strength coefficient</param>
	Spring(Rigidbody* body1, Rigidbody* body2, float damping, float springCoefficient);
	~Spring();
	/// <summary>
	/// Applies forces to push or pull the bodies towards or away from eachother
	/// </summary>
	/// <param name="gravity">The direction and magnitude of gravity</param>
	/// <param name="timeStep">The time that has passed during this physics update</param>
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	/// <summary>
	/// Draws the spring
	/// </summary>
	virtual void Draw();

protected:
	/// <summary>
	/// The first body attached to the spring
	/// </summary>
	Rigidbody* m_body1;
	/// <summary>
	/// The second body attached to the spring
	/// </summary>
	Rigidbody* m_body2;
	/// <summary>
	/// The attachment point on the first body in local co-ordinates relative to the body
	/// </summary>
	glm::vec2 m_contact1;
	/// <summary>
	/// The attachment point on the second body in local co-ordinates relative to the body
	/// </summary>
	glm::vec2 m_contact2;
	/// <summary>
	/// The colour of the spring
	/// </summary>
	glm::vec4 m_colour;
	/// <summary>
	/// The dampening factor for the springs
	/// </summary>
	float m_damping;
	/// <summary>
	/// The length at which the spring applies no force
	/// </summary>
	float m_restLength;
	/// <summary>
	/// The springs strength
	/// </summary>
	float m_springCoefficient;
};

