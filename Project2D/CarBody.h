#pragma once
#include "Box.h"
#include "Wheel.h"

class CarBody : public Box
{
public:
	CarBody();
	~CarBody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

protected:
	Wheel* m_backWheel;
	Wheel* m_frontWheel;
};

