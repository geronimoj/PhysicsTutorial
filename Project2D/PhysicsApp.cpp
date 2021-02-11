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
#include "Softbody.h"

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
	Plane* plane1 = new Plane(glm::vec2(0, 1), -50, glm::vec4(0, 0, 1, 1));
	Plane* plane2 = new Plane(glm::vec2(1, 0), -100, glm::vec4(0, 0, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(-1, 0), -100, glm::vec4(0, 0, 1, 1));
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);

	SphereTest();

	////Sphere* ball1 = new Sphere(glm::vec2(0, 100), true, glm::vec2(50, 30), 3.0f, 5, glm::vec4(1, 0, 0, 1));
	////Sphere* ball2 = new Sphere(glm::vec2(0, 30), false, glm::vec2(10, -50), 3.0f, 5, glm::vec4(0, 1, 0, 1));
	///*Sphere* ball3 = new Sphere(glm::vec2(0, 20), false, glm::vec2(10, -50), 3.0f, 5, glm::vec4(0, 1, 0, 1));
	//Sphere* ball4 = new Sphere(glm::vec2(0, 10), false, glm::vec2(10, -50), 3.0f, 5, glm::vec4(0, 1, 0, 1));
	//Sphere* ball5 = new Sphere(glm::vec2(0, 0), false, glm::vec2(10, -50), 3.0f, 5, glm::vec4(0, 1, 0, 1));
	//Sphere* ball6 = new Sphere(glm::vec2(0, -10), false, glm::vec2(10, -50), 3.0f, 5, glm::vec4(0, 1, 0, 1));
	//Sphere* ball7 = new Sphere(glm::vec2(0, -20), false, glm::vec2(10, -50), 3.0f, 5, glm::vec4(0, 1, 0, 1));
	//Sphere* ball8 = new Sphere(glm::vec2(0, -30), false, glm::vec2(10, -50), 3.0f, 5, glm::vec4(0, 1, 0, 1));
	//Sphere* ball9 = new Sphere(glm::vec2(0, -40), false, glm::vec2(10, -50), 3.0f, 5, glm::vec4(0, 1, 0, 1));*/
	//Box* box1 = new Box(glm::vec2(0, 0), false, glm::vec2(0, 0), 3.0f, glm::vec4(1, 1, 1, 1), glm::vec2(20, 20), 0, 0, 0, 0, 0.5f);
	//Box* box2 = new Box(glm::vec2(10, 20), false, glm::vec2(0, 0), 3.0f, glm::vec4(1, 1, 1, 1), glm::vec2(20, 10), 0, 0, 0, 0, 1);
	////Spring* spring1 = new Spring(ball1, ball2, 15, glm::vec4(1,1,1,1), 10, 0.1f);
	///*Spring* spring2 = new Spring(ball2, ball3, 15, glm::vec4(1, 1, 1, 1), 10, 0.1f);
	//Spring* spring3 = new Spring(ball3, ball4, 15, glm::vec4(1, 1, 1, 1), 10, 0.1f);
	//Spring* spring4 = new Spring(ball4, ball5, 15, glm::vec4(1, 1, 1, 1), 10, 0.1f);
	//Spring* spring5 = new Spring(ball5, ball6, 15, glm::vec4(1, 1, 1, 1), 10, 0.1f);
	//Spring* spring6 = new Spring(ball6, ball7, 15, glm::vec4(1, 1, 1, 1), 10, 0.1f);
	//Spring* spring7 = new Spring(ball7, ball8, 15, glm::vec4(1, 1, 1, 1), 10, 0.1f);
	//Spring* spring8 = new Spring(ball8, ball9, 15, glm::vec4(1, 1, 1, 1), 10, 0.1f);*/

	////m_physicsScene->AddActor(ball1);
	////m_physicsScene->AddActor(ball2);
	///*m_physicsScene->AddActor(ball3);
	//m_physicsScene->AddActor(ball4);
	//m_physicsScene->AddActor(ball5);
	//m_physicsScene->AddActor(ball6);
	//m_physicsScene->AddActor(ball7);
	//m_physicsScene->AddActor(ball8);
	//m_physicsScene->AddActor(ball9);*/
	///*m_physicsScene->AddActor(spring1);
	//m_physicsScene->AddActor(spring2);
	//m_physicsScene->AddActor(spring3);
	//m_physicsScene->AddActor(spring4);
	//m_physicsScene->AddActor(spring5);
	//m_physicsScene->AddActor(spring6);
	//m_physicsScene->AddActor(spring7);
	//m_physicsScene->AddActor(spring8);*/
	//m_physicsScene->AddActor(plane1);
	//m_physicsScene->AddActor(plane2);
	//m_physicsScene->AddActor(plane3);
	//m_physicsScene->AddActor(box1);
	//m_physicsScene->AddActor(box2);

	//std::vector<std::string> sb;
	//sb.push_back("00..00");
	//sb.push_back("00..00");
	//sb.push_back("00..00");
	//sb.push_back("00..00");
	//sb.push_back("00..00");

	////Softbody::Build(m_physicsScene,glm::vec2(50,-20), 5, 10, 10, sb);

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

void PhysicsApp::SphereTest()
{
	//m_physicsScene->AddActor(new Sphere(glm::vec2(-30, 30), false, glm::vec2(0, 20), 3.0f, 15, glm::vec4(0, 1, 0, 1), -10));
	m_physicsScene->AddActor(new Sphere(glm::vec2(50, -35), false, glm::vec2(0, 0), 3.0f, 15, glm::vec4(0, 1, 0, 1), 200));
}
