#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "PhysicsObject.h"

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	void RemoveActor(PhysicsObject* actor, bool doDelete = false);
	void Update(float dt);
	void Draw();

	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 GetGravity() const { return m_gravity; }

	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float GetTimeStep() const { return m_timeStep; }

	void CheckForCollision();

	static bool Sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Box(PhysicsObject*, PhysicsObject*);
	static bool Plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool Plane2Box(PhysicsObject*, PhysicsObject*);
	static bool Box2Plane(PhysicsObject*, PhysicsObject*);
	static bool Box2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Box2Box(PhysicsObject*, PhysicsObject*);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};

