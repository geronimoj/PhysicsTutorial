#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
/// <summary>
/// The application the simulation is run inside
/// </summary>
class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	virtual ~PhysicsApp();
	/// <summary>
	/// The startup function for initialising the scene
	/// </summary>
	/// <returns></returns>
	virtual bool startup();
	/// <summary>
	/// Fancy destructor
	/// </summary>
	virtual void shutdown();
	/// <summary>
	/// Increments timer, calls update on the physics scene, calls draw and checks for the escape key to exit
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void update(float deltaTime);
	/// <summary>
	/// Draws the world
	/// </summary>
	virtual void draw();
	/// <summary>
	/// Spawns two spheres with different properties
	/// </summary>
	void SphereTest();
	/// <summary>
	/// Spawns a rolling sphere
	/// </summary>
	void RollingSphere();
	/// <summary>
	/// Spawns a falling box at an angle
	/// </summary>
	void FallingBox();
	/// <summary>
	/// Spawns a rotating box
	/// </summary>
	void RotatingBox();
	/// <summary>
	/// Spawns a varienty of physics objects and kinematic objcets
	/// </summary>
	void BounceHouse();

protected:
	/// <summary>
	/// Pointer to the renderer
	/// </summary>
	aie::Renderer2D*	m_2dRenderer;
	/// <summary>
	/// Pointer to the font used for text
	/// </summary>
	aie::Font*			m_font;
	/// <summary>
	/// A pointer to the Physics Scene
	/// </summary>
	PhysicsScene* m_physicsScene;
	/// <summary>
	/// The global timer
	/// </summary>
	float m_timer;
};