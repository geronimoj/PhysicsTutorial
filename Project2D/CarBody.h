#pragma once
#include "Box.h"
#include "Wheel.h"
/// <summary>
/// Creates and controls a car
/// </summary>
class CarBody : public Box
{
public:
	CarBody();
	~CarBody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

protected:
	Wheel* m_backWheel;
	Wheel* m_frontWheel;

	glm::vec2 m_frontOrigin;
	glm::vec2 m_backOrigin;
};

