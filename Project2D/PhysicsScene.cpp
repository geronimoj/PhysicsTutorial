#include <algorithm>
#include <iostream>
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

float PhysicsScene::m_timeStep = 0;
glm::vec2 PhysicsScene::m_gravity = glm::vec2(0);

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
/// <summary>
/// Contains pointers to the collision detection and resolution functions between two objects. Used for quick and easy access between collision functions
/// </summary>
static fn collisionFunctionArray[] =
{	//Plane						//Sphere					//Box
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere, PhysicsScene::Plane2Box,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere, PhysicsScene::Sphere2Box,
	PhysicsScene::Box2Plane, PhysicsScene::Box2Sphere, PhysicsScene::Box2Box
};
/// <summary>
/// Determines which layers can collide with which layers.
/// </summary>
static bool layerCollision[] =
{/*		0		1		2		*/
/*0*/	true,	true,	true,
/*1*/	true,	true,	false,
/*2*/	true,	false,	true
};
/// <summary>
/// The number of layers in the scene
/// </summary>
const unsigned int NUMBER_OF_LAYERS = 3;

PhysicsScene::PhysicsScene()
{
	m_gravity = glm::vec2(0);
	m_timeStep = 0.01f;
	//Loop through the layerCollisions array and even it out down the middle just in case someone misses a layer when adjusting stuff.
	//Also reduces workload.
	for (int x = 0; x < NUMBER_OF_LAYERS; x++)
		for (int y = 0; y < NUMBER_OF_LAYERS; y++)
		{	//If either value are true, set the other/both of them to be true
			if (layerCollision[(y * NUMBER_OF_LAYERS) + x] || layerCollision[(x * NUMBER_OF_LAYERS) + y])
			{
				layerCollision[(y * NUMBER_OF_LAYERS) + x] = true;
				layerCollision[(x * NUMBER_OF_LAYERS) + y] = true;
			}
		}
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

		std::cout << "Total Energy Of Simulation: " << getTotalEnergy() << std::endl;
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

			if ((int)object1->GetShapeID() < 0 || (int)object2->GetShapeID() < 0)
				continue;

			if (!layerCollision[(object1->GetLayer() * NUMBER_OF_LAYERS) + object2->GetLayer()] || !layerCollision[(object2->GetLayer() * NUMBER_OF_LAYERS) + object1->GetLayer()])
				continue;
			//Using the function pointer array, we move our index forward and down the 1D array
			int functionIdx = ((int)object1->GetShapeID() * (int)ShapeType::SHAPE_COUNT) + (int)object2->GetShapeID();
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			//If the function is nullptr, then don't bother otherwise check the collision
			if (collisionFunctionPtr != nullptr)
				//Check for the collision
				collisionFunctionPtr(object1, object2);
		}
}

void PhysicsScene::ApplyContactForces(Rigidbody* actor1, Rigidbody* actor2, glm::vec2 norm, float pen)
{	//If actor2 is nullptr, then set body2Mass to INT_MAX
	//Otherwise its something so set it to its mass.
	
	//The ? operator determines what should be returned in the case of two possible options
	//The area before the ? operator must evaluated to true or false.
	//Afterwards, two options must be present, separated by : with the left being the true case.
	float body2Mass = actor2 ? actor2->GetMass() : INT_MAX;

	float body1Factor = body2Mass / (actor1->GetMass() + body2Mass);

	actor1->SetPosition(actor1->GetPosition() - body1Factor * norm * pen);
	//If actor2 is nullptr, then this will be skipped
	if (actor2)
		actor2->SetPosition(actor2->GetPosition() + (1 - body1Factor) * norm * pen);
}

float PhysicsScene::GetMass0(const Rigidbody& actor, float r)
{
	return 1.0f / (1.0f / actor.GetMass() + (r * r) / actor.GetMoment());
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

			float penetration = sphere1->GetRadius() + sphere2->GetRadius() - dist;
			if (penetration > 0)
			sphere1->ResolveCollision(sphere2, 0.5f * (sphere1->GetPosition() + sphere2->GetPosition()), nullptr, penetration);
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
				plane->ResolveCollision(sphere, sphere->GetPosition() - sphere->GetRadius() * plane->GetNormal());

				return true;
			}
		}
	}
	return false;
}

bool PhysicsScene::Sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);

	if (sphere != nullptr && box != nullptr)
	{
		glm::vec2 circlePosWorld = sphere->GetPosition() - box->GetPosition();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->GetLocalX()), glm::dot(circlePosWorld, box->GetLocalY()));

		glm::vec2 closestPointOnBoxBox = circlePosBox;
		glm::vec2 extents = box->GetExtents();

		if (closestPointOnBoxBox.x < -extents.x) 
			closestPointOnBoxBox.x = -extents.x;
		if (closestPointOnBoxBox.x > extents.x) 
			closestPointOnBoxBox.x = extents.x; 
		if (closestPointOnBoxBox.y < -extents.y) 
			closestPointOnBoxBox.y = -extents.y; 
		if (closestPointOnBoxBox.y > extents.y) 
			closestPointOnBoxBox.y = extents.y;

		glm::vec2 closetsPointOnBoxWorld = box->GetPosition() + closestPointOnBoxBox.x * box->GetLocalX() + closestPointOnBoxBox.y * box->GetLocalY();
		glm::vec2 circleToBox = sphere->GetPosition() - closetsPointOnBoxWorld;

		float penetration = sphere->GetRadius() - glm::length(circleToBox);

		if (penetration > 0)
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closetsPointOnBoxWorld;
			box->ResolveCollision(sphere, contact, &direction, penetration);
			return true;
		}
	}

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

bool PhysicsScene::Box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* plane = dynamic_cast<Plane*>(obj2);
	Box* box = dynamic_cast<Box*>(obj1);

	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;

		glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();

		for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
			for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
			{
				glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();

				glm::vec2 p = box->GetPosition() + displacement;

				float distFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());

				glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity() * glm::vec2(-displacement.y, displacement.x);

				float velocityIntoPlane = glm::dot(pointVelocity, plane->GetNormal());

				if (distFromPlane < 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}

		if (numContacts > 0)
		{
			plane->ResolveCollision(box, contact / (float)numContacts);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Sphere2Box(obj2, obj1);
}

bool PhysicsScene::Box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);

	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 boxPos = box2->GetPosition() - box1->GetPosition();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;
		box1->CheckBoxCorners(*box2, contact, numContacts, pen, norm);
		if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, norm))
			norm = -norm;
		if (pen > 0)
			box1->ResolveCollision(box2, contact / (float)numContacts, &norm, pen);
		return true;
	}

	return false;
}

float PhysicsScene::getTotalEnergy()
{
	float total = 0.0f;
	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
	{
		PhysicsObject* obj = *it;
		total += obj->getEnergy();
	}
	return total;
}
