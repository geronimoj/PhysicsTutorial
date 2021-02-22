#pragma once
#include <glm/glm.hpp>
/// <summary>
/// Used to determine what type of shape/objcet each PhysicsObject is
/// </summary>
enum class ShapeType
{
	SPRING = -1,
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
	SHAPE_COUNT = 3
};
/// <summary>
/// The base class for all objects in the scene that should be simulated by or interact with physics
/// </summary>
class PhysicsObject
{
protected:
	/// <summary>
	/// Creates an object. Must be called from a derived class
	/// </summary>
	/// <param name="shapeID">The type of shape. See ShapeType for possible shapes</param>
	/// <param name="layer">The layer this physics object exists on. Determines what it can collide with</param>
	/// <param name="elasticity">The elasticity factor of this object. Set to 1 for no energy loss or gain</param>
	/// <param name="staticFriction">The static friction factor</param>
	/// <param name="kinematicFriction">The kinetic friction factor</param>
	PhysicsObject(ShapeType shapeID, unsigned int layer, float elasticity, float staticFriction, float kinematicFriction) : m_shapeID(shapeID), m_layer(layer), m_elasticity(elasticity), m_staticFrictionCo(staticFriction), m_kinematicFrictionCo(kinematicFriction) {}

public:
	/// <summary>
	/// Is a pure virtual function. The physics scene calls this fixed update.
	/// </summary>
	/// <param name="gravity">The direction and magnitude of gravity</param>
	/// <param name="timeStep">The amount of time that has passed this physics update</param>
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	/// <summary>
	/// Is a pure virtual function. Draws the PhysicsObject
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// Empty function for derived classes
	/// </summary>
	virtual void ResetPosition() {};
	/// <summary>
	/// Returns the shape ID
	/// </summary>
	/// <returns>Returns the shape ID as a ShapeType.</returns>
	ShapeType GetShapeID() { return m_shapeID; }
	/// <summary>
	/// Returns the elasticity factor
	/// </summary>
	/// <returns>Returns the elasticity factor as a float</returns>
	float GetElasticity() { return m_elasticity; }
	/// <summary>
	/// Returns the staic friction coefficient of this object
	/// </summary>
	/// <returns> Returns the staic friction coefficient of this object as a float</returns>
	float GetStaticFriction() { return m_staticFrictionCo; }
	/// <summary>
	/// Returns the kinetic friction coefficient of this object
	/// </summary>
	/// <returns>Returns the kinetic friction coefficient of this object as a float</returns>
	float GetKinematicFriction() { return m_kinematicFrictionCo; }
	/// <summary>
	/// Returns the layer this PhysicsObject exists on. Determines what it can collide with
	/// </summary>
	/// <returns>Returns the layer this PhysicsObject exists on as an unsigned int</returns>
	unsigned int GetLayer() const { return m_layer; }
	/// <summary>
	/// The minimum linear velocity of a physics object. Having a velocity below this value will result in velocity being set to 0,0
	/// </summary>
	static const float MIN_LINEAR_THRESHOLD;
	/// <summary>
	/// The minimum angular velocity of a physics object. Having angular velocity below this value will result in that velocity being set to 0
	/// </summary>
	static const float MIN_ANGULAR_THRESHOLD;
	/// <summary>
	/// Returns the energy of the object. Returns 0 by default unless a derived class overrides it
	/// </summary>
	virtual float getEnergy() { return 0; }

protected:
	/// <summary>
	/// The Shape ID of the object
	/// </summary>
	ShapeType m_shapeID;
	/// <summary>
	/// The elasticity coefficient
	/// </summary>
	float m_elasticity;
	/// <summary>
	/// The static friction coefficient
	/// </summary>
	float m_staticFrictionCo;
	/// <summary>
	/// The kinetic friction coefficient
	/// </summary>
	float m_kinematicFrictionCo;
	/// <summary>
	/// The layer this object exists on. Determines what it can collide with
	/// </summary>
	unsigned int m_layer;
};

