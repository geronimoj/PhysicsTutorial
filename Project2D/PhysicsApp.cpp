#pragma once

#include <Gizmos.h>
#include <glm\ext.hpp>
#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"
#include "Wheel.h"
#include "Softbody.h"
#include "CarBody.h"

PhysicsApp::PhysicsApp()  : m_physicsScene(nullptr), m_2dRenderer(nullptr), m_font(nullptr), m_timer(0), m_currentScene(0)
{
}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() {

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_timer = 0;
	//Create the Physics Scene and set up its default values
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, -100));
	m_physicsScene->SetTimeStep(0.01f);

	LoadScene();

	/*
	Game Ideas:
	 - Angry Birds
	 - Drive Ahead
	 - Sumo Wrestling with balls
	 - 2D Bowling
	 - Soft Body sym
	 - Ball Square & Kinetic Object in cage with a little bit of energy loss
	 - Box friction showcase 1 (two boxes colliding without gravity)
	 - Box friction showcase 2 (lone box landing on corner without elasticity)
	 */

	return true;
}

void PhysicsApp::shutdown() {
	
	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

void PhysicsApp::update(float deltaTime) {

	//deltaTime /= 10;
	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	if (input->wasKeyPressed(aie::INPUT_KEY_UP))
	{
		m_currentScene++;
		LoadScene();
	}
	else if (input->wasKeyPressed(aie::INPUT_KEY_DOWN))
	{
		m_currentScene--;
		LoadScene();
	}

	aie::Gizmos::clear();
	//Let the PhysicsScene do its thing
	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw(m_2dRenderer);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	//Store the original camera position (to avoid moving UI)
	float x, y;
	m_2dRenderer->getCameraPos(x, y);
	//Reset the camera so the UI is correct
	m_2dRenderer->setCameraPos(0, 0);

	// begin drawing sprites
	m_2dRenderer->begin();

	static float aspectRation = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRation, 100 / aspectRation, -1, 1));
	// output some text, uses the last used colour
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Press Up or Down arrows to change scene!", 0, 720 - 64);
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 96);
	//If we are in the car scene, write the controls
	if (m_currentScene == 0)
		m_2dRenderer->drawText(m_font, "Press W or S to move forward or backwards!", 0, 720 - 128);

	// done drawing sprites
	m_2dRenderer->end();
	//Reset the camera position for object rendering
	m_2dRenderer->setCameraPos(x, y);
}

void PhysicsApp::LoadScene()
{	//Backup check to ensure we have a physics scene
	if (m_physicsScene == nullptr)
		return;
	//If there are actors still in the scene, delete them
	if (m_physicsScene->HasActors())
		m_physicsScene->RemoveAllActors();

	m_2dRenderer->setCameraPos(0, 0);
	//Load the correct scene
	switch (m_currentScene)
	{
		//If there is no case, eg m_currentScene is not a valid scene, default to scene 0. This in turn allows us to clamp the default scene as well
	default:
	case 0:
		Car();
		//Set current scene to be this scene just in case of default
		m_currentScene = 0;
		break;
	case 1:
		BounceHouse();
		break;
	case 2:
		SphereTest();
		break;
	case 3:
		RollingSphere();
		break;
	case 4:
		FallingBox();
		break;
	case -1:
		//We repeat the clamping idea where we used default above but use an index of -1, assuming the user loops around from the bottom to the top
	case 5:
		RotatingBox();
		m_currentScene = 5;
		break;
	}
}

void PhysicsApp::SphereTest()
{
	//Create the walls and floor
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50, 0, glm::vec4(0, 0, 1, 1), 0.9f);
	Plane* plane2 = new Plane(glm::vec2(1, 0), -100, 0, glm::vec4(0, 0, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(-1, 0), -100, 0, glm::vec4(0, 0, 1, 1));
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	//Generates two spheres
	m_physicsScene->AddActor(new Sphere(glm::vec2(-30, 30), false, glm::vec2(60, 0), 3.0f, 15, glm::vec4(0, 1, 0, 1), 0, 0, 0.1f, 0.1f, 0.9f));
	m_physicsScene->AddActor(new Sphere(glm::vec2(30, 5), false, glm::vec2(-60, 0), 3.0f, 15, glm::vec4(0, 1, 0, 1), 0, 0, 0.1f, 0.1f, 0.9f));
}

void PhysicsApp::RollingSphere()
{
	//Create the walls and floor
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50, 0, glm::vec4(0, 0, 1, 1), 0);
	Plane* plane2 = new Plane(glm::vec2(1, 0), -100, 0, glm::vec4(0, 0, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(-1, 0), -100, 0, glm::vec4(0, 0, 1, 1));
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	//Create a rolling sphere
	m_physicsScene->AddActor(new Sphere(glm::vec2(0, 0), false, glm::vec2(0, 0), 3.0f, 15, glm::vec4(0, 1, 0, 1), 30, 0, 0, 1, 0));
}

void PhysicsApp::FallingBox()
{
	//Create the walls and floor
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50, 0, glm::vec4(0, 0, 1, 1), 0);
	Plane* plane2 = new Plane(glm::vec2(1, 0), -100, 0, glm::vec4(0, 0, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(-1, 0), -100, 0, glm::vec4(0, 0, 1, 1));
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	//Creates a box on an angle
	m_physicsScene->AddActor(new Box(glm::vec2(-20, 50), false, glm::vec2(0, 0), 3.0f, glm::vec4(1, 0, 1, 1), glm::vec2(20, 10), 45, 0, 0, 0, 1, 0));
}

void PhysicsApp::RotatingBox()
{
	//Create the walls and floor
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50, 0, glm::vec4(0, 0, 1, 1));
	Plane* plane2 = new Plane(glm::vec2(1, 0), -100, 0, glm::vec4(0, 0, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(-1, 0), -100, 0, glm::vec4(0, 0, 1, 1));
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	//Creates a rotating box
	m_physicsScene->AddActor(new Box(glm::vec2(30, 20), false, glm::vec2(-10, 0), 3.0f, glm::vec4(1, 1, 1, 1), glm::vec2(30, 15), 0, 10, 0, 0, 0, 1));

}

void PhysicsApp::BounceHouse()
{
	//Create the walls and floor
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50, 0, glm::vec4(0, 0, 1, 1));
	Plane* plane2 = new Plane(glm::vec2(1, 0), -100, 0, glm::vec4(0, 0, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(-1, 0), -100, 0, glm::vec4(0, 0, 1, 1));
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	//Create a kinematic box and sphere
	//I should really create a constructor for creating kinematic boxes and spheres
	m_physicsScene->AddActor(new Box(glm::vec2(-75, -25), true, glm::vec2(0), 0, glm::vec4(1, 0, 1, 1), glm::vec2(25, 5), 0, 0));
	m_physicsScene->AddActor(new Sphere(glm::vec2(50, 15), true, glm::vec2(0), 0, 15, glm::vec4(1, 0, 1, 1)));
	//Create non kinematic objects
	//Two boxes, each above the edge of the kinematic objects
	m_physicsScene->AddActor(new Box(glm::vec2(-25, 0), false, glm::vec2(0), 3, glm::vec4(1, 1, 1, 1), glm::vec2(30, 10), 0, 0));
	m_physicsScene->AddActor(new Box(glm::vec2(30, 50), false, glm::vec2(0), 3, glm::vec4(1, 1, 1, 1), glm::vec2(20, 10), 10, 1));
	//Two spheres, close to the ground with a bit of rotation to make things interesting
	m_physicsScene->AddActor(new Sphere(glm::vec2(70, -20), false, glm::vec2(-40, 60), 6, 10, glm::vec4(0, 1, 1, 1), 10));
	m_physicsScene->AddActor(new Sphere(glm::vec2(-70, 20), false, glm::vec2(0, 20), 6, 10, glm::vec4(0, 1, 1, 1), 10));
}

void PhysicsApp::Car()
{
	//Create the walls and floor
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50, 0, glm::vec4(0, 0, 1, 1), 0);
	Plane* plane2 = new Plane(glm::vec2(0.5, 0.866025404), -50, 0, glm::vec4(0, 0, 1, 1), 0);
	Plane* plane3 = new Plane(glm::vec2(-1, 0), -650 - 200, 0, glm::vec4(0, 0, 1, 1));
	Plane* plane4 = new Plane(glm::vec2(1, 0), -100, 0, glm::vec4(0, 0, 1, 1), 0);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	m_physicsScene->AddActor(plane4);
	//Add a car to the scene
	CarBody* car = new CarBody(m_physicsScene, 2, glm::vec2(0), glm::vec2(15, 7), glm::vec2(10, -7), glm::vec2(-10, -7), 3, glm::vec4(0, 1, 1, 1), glm::vec4(1, 1, 1, 1),
		1, 3, 200, 1000, 0.1f, 0.1f, 0.1f);

	car->SetCamera(m_2dRenderer);

	m_physicsScene->AddActor(car);

	Box* ramp1 = new Box(glm::vec2(50, -50), true, glm::vec2(0), 0, glm::vec4(1, 1, 0, 1), glm::vec2(100, 5), 20, 0, 0,0,0,0);
	Box* floor1 = new Box(glm::vec2(193.97 , -15.8), true, glm::vec2(0), 0, glm::vec4(1, 1, 0, 1), glm::vec2(50, 5), 0, 0, 0, 0, 0, 0);
	Box* ramp2 = new Box(glm::vec2(243.97, -15.8), true, glm::vec2(0), 0, glm::vec4(1, 1, 0, 1), glm::vec2(5, 5), 10, 0, 0, 0, 0, 0);
	Box* ramp3 = new Box(glm::vec2(250.97, -13.8), true, glm::vec2(0), 0, glm::vec4(1, 1, 0, 1), glm::vec2(5, 5), 30, 0, 0, 0, 0, 0);
	Box* ramp4 = new Box(glm::vec2(400 + 200, -50), true, glm::vec2(0), 0, glm::vec4(1, 1, 0, 1), glm::vec2(100, 5), 20, 0, 0, 0, 0, 0);
	Box* ramp5 = new Box(glm::vec2(470 + 200, -20), true, glm::vec2(0), 0, glm::vec4(1, 1, 0, 1), glm::vec2(100, 5), 30, 0, 0, 0, 0, 0);
	Box* ramp6 = new Box(glm::vec2(500 + 200, 0), true, glm::vec2(0), 0, glm::vec4(1, 1, 0, 1), glm::vec2(100, 5), 40, 0, 0, 0, 0, 0);
	Box* ramp7 = new Box(glm::vec2(510 + 200, 20), true, glm::vec2(0), 0, glm::vec4(1, 1, 0, 1), glm::vec2(100, 5), 50, 0, 0, 0, 0, 0);
	Box* floor2 = new Box(glm::vec2(610 + 200, 94), true, glm::vec2(0), 0, glm::vec4(1, 1, 0, 1), glm::vec2(40, 5), 0, 0, 0, 0, 0, 0);
	Box* bouncy1 = new Box(glm::vec2(300, -50), true, glm::vec2(0), 0, glm::vec4(0, 1, 0, 1), glm::vec2(50, 10), 0, 0, 0, 0, 0, 2);
	ramp1->SetStatic(true);
	floor1->SetStatic(true);
	floor2->SetStatic(true);
	ramp2->SetStatic(true);
	ramp3->SetStatic(true);
	ramp4->SetStatic(true);
	ramp5->SetStatic(true);
	ramp6->SetStatic(true);
	ramp7->SetStatic(true);
	bouncy1->SetStatic(true);
	m_physicsScene->AddActor(ramp1);
	m_physicsScene->AddActor(floor1);
	m_physicsScene->AddActor(floor2);
	m_physicsScene->AddActor(ramp2);
	m_physicsScene->AddActor(ramp3);
	m_physicsScene->AddActor(ramp4);
	m_physicsScene->AddActor(ramp5);
	m_physicsScene->AddActor(ramp6);
	m_physicsScene->AddActor(ramp7);
	m_physicsScene->AddActor(bouncy1);
}
