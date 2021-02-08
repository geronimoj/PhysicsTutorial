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

PhysicsApp::PhysicsApp()  : m_physicsScene(nullptr), m_2dRenderer(nullptr), m_font(nullptr), m_timer(0)
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
	//Create the walls and floor
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50, glm::vec4(0, 0, 1, 1), 0.5f);
	Plane* plane2 = new Plane(glm::vec2(1, 0), -100, glm::vec4(0, 0, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(-1, 0), -100, glm::vec4(0, 0, 1, 1));

	Sphere* ball1 = new Sphere(glm::vec2(-40, 0), false, glm::vec2(10, 30), 3.0f, 10, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(40, 0), false, glm::vec2(-10, 30), 3.0f, 10, glm::vec4(0, 1, 0, 1));
	//Box* box1 = new Box(glm::vec2(0, 0), false, glm::vec2(0, 0), 3.0f, glm::vec4(1, 1, 1, 1), glm::vec2(20, 10), 0, 0, 0, 0, 0.5f);
	//Box* box2 = new Box(glm::vec2(0, 50), false, glm::vec2(0, 0), 3.0f, glm::vec4(1, 1, 1, 1), glm::vec2(20, 10), 0, 0, 0, 0, 0.5f);
	Spring* spring1 = new Spring(ball1, ball2, 80, glm::vec4(1,1,1,1), 0.1f);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(spring1);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	//m_physicsScene->AddActor(box1);
	//m_physicsScene->AddActor(box2);

	return true;
}

void PhysicsApp::shutdown() {
	
	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

void PhysicsApp::update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();
	//Let the PhysicsScene do its thing
	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	static float aspectRation = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRation, 100 / aspectRation, -1, 1));
	// output some text, uses the last used colour
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 64);

	// done drawing sprites
	m_2dRenderer->end();
}