#pragma once
#include "Rigidbody.h"
class Box :	public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec4 colour, glm::vec2 extents, float orientation, float angularVelocity, float linearDrag, float angularDrag);
	~Box();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Draw();

	bool CheckBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);

	glm::vec2 GetExtents() { return m_extents; }
	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }
	float GetWidth() { return m_extents.x * 2; }
	float GetHeight() { return m_extents.y * 2; }

protected:
	glm::vec2 m_extents;
	glm::vec4 m_colour;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

