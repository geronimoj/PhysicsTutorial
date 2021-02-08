#pragma once
#include "Rigidbody.h"
class Sphere : public Rigidbody
{
public:
	Sphere(glm::vec2 position, bool isKinematic, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, float angularVelocity = 0, float linearDrag = 0, float angularDrag = 0, float elasticity = 1);
	~Sphere();

	virtual void Draw();

	float GetRadius() const { return m_radius; }
	glm::vec4 GetColour() const { return m_colour; }

protected:
	float m_radius;
	glm::vec4 m_colour;
};

