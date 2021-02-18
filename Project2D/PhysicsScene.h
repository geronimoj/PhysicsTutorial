#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "PhysicsObject.h"
#include "Rigidbody.h"

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
	static glm::vec2 GetGravity() { return m_gravity; }

	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	static float GetTimeStep() { return m_timeStep; }

	void CheckForCollision();

	static void ApplyContactForces(Rigidbody* actor1, Rigidbody* actor2, glm::vec2 norm, float pen);
	static float GetMass0(const Rigidbody& actor, float r);

	static bool Sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Box(PhysicsObject*, PhysicsObject*);
	static bool Plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool Plane2Box(PhysicsObject*, PhysicsObject*);
	static bool Box2Plane(PhysicsObject*, PhysicsObject*);
	static bool Box2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Box2Box(PhysicsObject*, PhysicsObject*);

	float getTotalEnergy();

protected:
	static glm::vec2 m_gravity;
	static float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};

