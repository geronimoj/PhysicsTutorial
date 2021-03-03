#pragma once
#include "Box.h"
#include "Wheel.h"
#include "PhysicsScene.h"
#include "Renderer2D.h"
/// <summary>
/// Creates and controls a car
/// </summary>
class CarBody : public Box
{
public:
	CarBody(PhysicsScene* scene, unsigned int layer, glm::vec2 position, glm::vec2 bodyExtents, glm::vec2 frontWheel, glm::vec2 backWheel, float mass, glm::vec4 bodyColour, glm::vec4 wheelColour, float wheelMass, float wheelRadius, float wheelAngAccel, float wheelAngVelLimit, float linearDrag, float angularDrag, float wheelElasticity);
	//Since we add the wheels to the scenes actors, we don't have to deal with cleaning them up :D
	~CarBody();
	/// <summary>
	/// Gives the car a reference to the camera
	/// </summary>
	/// <param name="renderer">A pointer to the camera</param>
	void SetCamera(aie::Renderer2D* renderer) { m_renderer = renderer; }

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

protected:
	Wheel* m_backWheel;
	Wheel* m_frontWheel;

	aie::Renderer2D* m_renderer;

	glm::vec2 m_frontOrigin;
	glm::vec2 m_backOrigin;
};

