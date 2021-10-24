#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Renderer2D.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"
/// <summary>
/// Simulates, draws all actors and checks for and resolves collisions.
/// For layer collisions, see PhysicsScene.cpp
/// </summary>
class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();
	/// <summary>
	/// Adds an actor to the scene
	/// </summary>
	/// <param name="actor">The actor to add</param>
	void AddActor(PhysicsObject* actor);
	/// <summary>
	/// Removes an actor from the scene. If doDelete is true, deletes it as well
	/// </summary>
	/// <param name="actor">The actor to delete</param>
	/// <param name="doDelete">Should the actor also be deleted?</param>
	void RemoveActor(PhysicsObject* actor, bool doDelete = false);
	/// <summary>
	/// Deletes all the actors in the scene.
	/// </summary>
	void RemoveAllActors();
	/// <summary>
	/// Simulates the physics simulation
	/// </summary>
	/// <param name="dt">Delta time</param>
	void Update(float dt);
	/// <summary>
	/// Draws the scene
	/// </summary>
	void Draw(aie::Renderer2D* rend);
	/// <summary>
	/// Sets the direction and magnitude of gravity
	/// </summary>
	/// <param name="gravity">The direction and magnitude of gravity</param>
	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	/// <summary>
	/// Returns gravity
	/// </summary>
	/// <returns>Returns gravity as a vector 2 with direction and magnitude</returns>
	static glm::vec2 GetGravity() { return m_gravity; }
	/// <summary>
	/// Sets the timestep to the given value
	/// </summary>
	/// <param name="timeStep">Sets the timestep</param>
	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	/// <summary>
	/// Returns the time step
	/// </summary>
	/// <returns>Returns the timestep as a float</returns>
	static float GetTimeStep() { return m_timeStep; }
	/// <summary>
	/// Checks for collisions amongst all actors in m_actors
	/// </summary>
	void CheckForCollision();
	/// <summary>
	/// Forcefully teleports two actors away from eachother so they are no-long clipping into eachother
	/// </summary>
	/// <param name="actor1">Actor 1</param>
	/// <param name="actor2">Actor 2</param>
	/// <param name="norm">The normal of the actors collision</param>
	/// <param name="pen">How much the actors are penetrating eachother along the normal</param>
	static void ApplyContactForces(Rigidbody* actor1, Rigidbody* actor2, glm::vec2 norm, float pen);
	/// <summary>
	/// Calculates the mass at a point r from the center of mass of the actor
	/// </summary>
	/// <param name="actor">The actor to calculate mass0 for</param>
	/// <param name="r">The radius to the point</param>
	/// <returns>Returns mass0</returns>
	static float GetMass0(const Rigidbody& actor, float r);

	/// <summary>
	/// Determines if a sphere a sphere are colliding. If so, resolves the collision
	/// </summary>
	/// <param name="">Object 1</param>
	/// <param name="">Object 2</param>
	/// <returns>Returns true if the two objects were colliding</returns>
	static bool Sphere2Sphere(PhysicsObject*, PhysicsObject*);
	/// <summary>
	/// Determines if a sphere and a plane are colliding. If so, resolves the collision
	/// </summary>
	/// <param name="">Object 1</param>
	/// <param name="">Object 2</param>
	/// <returns>Returns true if the two objects were colliding</returns>
	static bool Sphere2Plane(PhysicsObject*, PhysicsObject*);
	/// <summary>
	/// Determines if a box and a sphere are colliding. If so, resolves the collision
	/// </summary>
	/// <param name="">Object 1</param>
	/// <param name="">Object 2</param>
	/// <returns>Returns true if the two objects were colliding</returns>
	static bool Sphere2Box(PhysicsObject*, PhysicsObject*);
	/// <summary>
	/// Determines if a sphere and a plane are colliding. If so, resolves the collision
	/// </summary>
	/// <param name="">Object 1</param>
	/// <param name="">Object 2</param>
	/// <returns>Returns true if the two objects were colliding</returns>
	static bool Plane2Sphere(PhysicsObject*, PhysicsObject*);
	/// <summary>
	/// Does nothing. Only exists to even out the function array in the .cpp
	/// </summary>
	/// <param name="">Object 1</param>
	/// <param name="">Object 2</param>
	/// <returns>Always returns false</returns>
	static bool Plane2Plane(PhysicsObject*, PhysicsObject*);
	/// <summary>
	/// Determines if a box and a plane are colliding. If so, resolves the collision
	/// </summary>
	/// <param name="">Object 1</param>
	/// <param name="">Object 2</param>
	/// <returns>Returns true if the two objects were colliding</returns>
	static bool Plane2Box(PhysicsObject*, PhysicsObject*);
	/// <summary>
	/// Determines if a box and a plane are colliding. If so, resolves the collision
	/// </summary>
	/// <param name="">Object 1</param>
	/// <param name="">Object 2</param>
	/// <returns>Returns true if the two objects were colliding</returns>
	static bool Box2Plane(PhysicsObject*, PhysicsObject*);
	/// <summary>
	/// Determines if a box and a sphere are colliding. If so, resolves the collision
	/// </summary>
	/// <param name="">Object 1</param>
	/// <param name="">Object 2</param>
	/// <returns>Returns true if the two objects were colliding</returns>
	static bool Box2Sphere(PhysicsObject*, PhysicsObject*);
	/// <summary>
	/// Determines if two boxes are colliding. If so, resolves the collision
	/// </summary>
	/// <param name="">Object 1</param>
	/// <param name="">Object 2</param>
	/// <returns>Returns true if two object were colliding</returns>
	static bool Box2Box(PhysicsObject*, PhysicsObject*);
	/// <summary>
	/// Returns the total energy in the scene for debugging purposes
	/// </summary>
	/// <returns>Returns the total energy in the scene as a float for debugging purposes</returns>
	float getTotalEnergy();

	bool HasActors() { return m_actors.size() != 0; }

protected:
	/// <summary>
	/// The direction and magnitude of gravity
	/// </summary>
	static glm::vec2 m_gravity;
	/// <summary>
	/// The time step for each physics simulation.
	/// </summary>
	static float m_timeStep;
	/// <summary>
	/// A reference to every actor in the scene
	/// </summary>
	std::vector<PhysicsObject*> m_actors;
};

