#include <algorithm>
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{	//Plane						//Sphere					//Box
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere, PhysicsScene::Plane2Box,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere, PhysicsScene::Sphere2Box,
	PhysicsScene::Box2Plane, PhysicsScene::Box2Sphere, PhysicsScene::Box2Box
};

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
{	//We store it as a static float because then it only initializes the memory once and said memory is re-used between calls
	static float accumulartedTime = 0.01f;
	accumulartedTime += dt;
	//While we can still simulate time, simulate it
	while (accumulartedTime >= m_timeStep)
	{	//Call update/move all the actors
		for (int i = 0; i < m_actors.size(); i++)
			m_actors[i]->FixedUpdate(m_gravity, m_timeStep);
		//Reduce the remaining time
		accumulartedTime -= m_timeStep;
		//Check for any collisions after this update loop
		CheckForCollision();
	}
}

void PhysicsScene::Draw()
{	//Loop through the actors and call their draw function
	for (int i = 0; i < m_actors.size(); i++)
		m_actors[i]->Draw();
}

void PhysicsScene::CheckForCollision()
{
	int actorCount = (int)m_actors.size();
	//Check for collisions
	//We loop through every actor and compare them with each other. Extremely innefficient
	for (int outer = 0; outer < actorCount - 1; outer++)
		for (int inner = outer + 1; inner < actorCount; inner++)
		{	//Get pointers to the objects
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			//Using the function pointer array, we move our index forward and down the 1D array
			int functionIdx = ((int)object1->GetShapeID() * (int)ShapeType::SHAPE_COUNT) + (int)object2->GetShapeID();
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			//If the function is nullptr, then don't bother otherwise check the collision
			if (collisionFunctionPtr != nullptr)
				//Check for the collision
				collisionFunctionPtr(object1, object2);
		}
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{	//Cast our physicsobjects to sphere types
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1); 
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
	//If they failed, one of them isn't a sphere
	if (sphere1 != nullptr && sphere2 != nullptr)
	{	//Get the distance between the spheres origins
		float dist = glm::distance(sphere1->GetPosition(), sphere2->GetPosition());
		//If the distance is less than the combined radius, they are colliding
		if (dist < sphere1->GetRadius() + sphere2->GetRadius())
		{	//Perform collision response algorythm
			sphere1->ResolveCollision(sphere2);
			return true;
		}
	}
	//No collision was performed
	return false;
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr)
	{	//Get the dot product to check if we are travelling into the plane
		float d = glm::dot(sphere->GetVelocity(), plane->GetNormal());
		//If the dot is < 0 then they are moving into eachother
		if (d < 0)
		{
			//Do the math from the tutorial. We re-use d to reduce memory usage
			d = glm::dot(sphere->GetPosition(), plane->GetNormal()) - plane->GetDistance() - sphere->GetRadius();

			if (d < 0)
			{	//Do propper collision
				plane->ResolveCollision(sphere);

				return true;
			}
		}
	}
	return false;
}

bool PhysicsScene::Sphere2Box(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Sphere2Plane(obj2, obj1);
}

bool PhysicsScene::Plane2Plane(PhysicsObject*, PhysicsObject*)
{	//Planes can't collide so don't do anything
	return false;
}

bool PhysicsScene::Plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Box2Plane(obj2, obj1);
}

bool PhysicsScene::Box2Plane(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Sphere2Box(obj2, obj1);
}

bool PhysicsScene::Box2Box(PhysicsObject*, PhysicsObject*)
{
	return false;
}
