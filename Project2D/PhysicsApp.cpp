#include <Gizmos.h>
#include <glm\ext.hpp>
#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

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

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01f);

	return true;
}

void PhysicsApp::shutdown() {
	
	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

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

	static float aspectRation = 16 / 9;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRation, 100 / aspectRation, -1, 1));
	// output some text, uses the last used colour
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 64);

	// done drawing sprites
	m_2dRenderer->end();
}