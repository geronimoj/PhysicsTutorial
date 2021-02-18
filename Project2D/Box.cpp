#pragma once
#include <Gizmos.h>
#include "Box.h"
#include "Rigidbody.h"

Box::Box(glm::vec2 position, bool isKinematic, glm::vec2 velocity, float mass, glm::vec4 colour, glm::vec2 extents, float orientation, float angularVelocity, float linearDrag, float angularDrag, float elasticity, float staticFriction, float kinematicFriction)
	: Rigidbody(ShapeType::BOX, position, isKinematic, velocity, orientation, mass, elasticity, angularVelocity, linearDrag, angularDrag, staticFriction, kinematicFriction), m_colour(colour), m_extents(extents), m_localX(glm::vec2(0,0)), m_localY(glm::vec2(0,0))
{
	m_moment = 1.0f / 12.0f * m_mass * GetWidth() * GetHeight();
}

void Box::FixedUpdate(glm::vec2 gravity, float timeStep)
{	//Call the bases Fixed Update
	Rigidbody::FixedUpdate(gravity, timeStep);

	float cs = cosf(m_orientation);
	float sn = sinf(m_orientation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::Draw()
{
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;

	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
	aie::Gizmos::add2DCircle(m_position, 1, 8, glm::vec4(0, 0, 0, 1));
}

bool Box::CheckBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal)
{
	float minX = 0, maxX = 0, minY = 0, maxY = 0;
	float boxW = box.GetWidth();
	float boxH = box.GetHeight();
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;

	for (float x = -box.GetExtents().x; x < boxW; x += boxW)
		for (float y = -box.GetExtents().y; y < boxH; y += boxH)
		{
			glm::vec2 p = box.GetPosition() + x * box.GetLocalX() + y * box.GetLocalY();

			glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));

			if (first || p0.x < minX) 
				minX = p0.x; 
			if (first || p0.x > maxX) 
				maxX = p0.x; 
			if (first || p0.y < minY) 
				minY = p0.y; 
			if (first || p0.y > maxY) 
				maxY = p0.y;

			if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
				p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}

	if (maxX <= -m_extents.x || minX >= m_extents.x ||
		maxY <= -m_extents.y || minY >= m_extents.y)
		return false;

	if (numLocalContacts == 0)
		return false;

	bool res = false;
	contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) / (float)numLocalContacts;
	numContacts++;

	float pen0 = m_extents.x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = m_localY;
		pen = pen0;
		res = true;
	}
	pen0 = maxY + m_extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = -m_localY;
		pen = pen0;
		res = true;
	}

	return res;
}
