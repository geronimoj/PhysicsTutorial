#pragma once
#include "Rigidbody.h"
class Box :	public Rigidbody
{
public:
	Box(glm::vec2 position, bool isKinematic, glm::vec2 velocity, float mass, glm::vec4 colour, glm::vec2 extents, float orientation, float angularVelocity, float linearDrag = 0, float angularDrag = 0, float elasticity = 1);
	~Box();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Draw();

	bool CheckBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);

	virtual glm::vec2 ToWorld(glm::vec2 offset) { return GetPosition() + GetLocalX() * offset.x + GetLocalY() * offset.y; }
	glm::vec2 GetExtents() const { return m_extents; }
	glm::vec2 GetLocalX() const { return m_localX; }
	glm::vec2 GetLocalY() const { return m_localY; }
	float GetWidth() const { return m_extents.x * 2; }
	/// <summary>
	/// Gets the height
	/// </summary>
	/// <returns></returns>
	float GetHeight() const { return m_extents.y * 2; }

protected:
	glm::vec2 m_extents;
	glm::vec4 m_colour;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

