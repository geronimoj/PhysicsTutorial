#pragma once
#include "Rigidbody.h"

class Spring : public PhysicsObject
{
public:
	Spring(Rigidbody* body1, Rigidbody* body2, float restLength, glm::vec4 colour,
		float springCoefficient, float damping = 0.1f, 
		glm::vec2 body1Contact = glm::vec2(0,0), glm::vec2 body2Contact = glm::vec2(0,0));
	Spring(Rigidbody* body1, Rigidbody* body2, float damping, float springCoefficient);
	~Spring();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Draw();

protected:
	Rigidbody* m_body1;
	Rigidbody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;
	glm::vec4 m_colour;

	float m_damping;
	float m_restLength;
	float m_springCoefficient;
};

