#include <algorithm>
#include "PhysicsScene.h"

PhysicsScene::PhysicsScene() : m_gravity(glm::vec2(0,0)), m_timeStep(0.01f)
{
}

PhysicsScene::~PhysicsScene()
{	//Call delete on all the actors in this scene
	for (int i = 0; i < m_actors.size(); i++)
		RemoveActor(m_actors[i], true);
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{	//Make sure the actor is valid before we add it to the scene
	if (actor != nullptr)
		m_actors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* actor, bool doDelete)
{	//If the actor is invalid, aka nullptr, simply return
	if (actor == nullptr)
		return;
	//Use the std::find function (in algorithm include) to find the actor
	std::vector<PhysicsObject*>::iterator it = std::find(m_actors.begin(), m_actors.end(), actor);
	//Make sure we are removing a valid actor
	if (it != m_actors.end())
		//Remove it
		m_actors.erase(it);
	//Check if we want to delete the actor as well
	if (doDelete)
		delete actor;
}

void PhysicsScene::Update(float dt)
{
	static float accumulartedTime = 0.01f;
	accumulartedTime += dt;

	while (accumulartedTime >= m_timeStep)
	{
		for (int i = 0; i < m_actors.size(); i++)
			m_actors[i]->FixedUpdate(m_gravity, m_timeStep);

		accumulartedTime -= m_timeStep;
	}
}

void PhysicsScene::Draw()
{	//Loop through the actors and call their draw function
	for (int i = 0; i < m_actors.size(); i++)
		m_actors[i]->Draw();
}