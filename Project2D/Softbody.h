#pragma once

#include <string>
#include "PhysicsScene.h"
/// <summary>
/// Generates a softbody out of springs and spheres.
/// </summary>
class Softbody
{
public:
	/// <summary>
	/// Creates a soft body.
	/// 0 Represents circle
	/// . represents open space
	/// </summary>
	/// <param name="scene">A reference to the physics scene</param>
	/// <param name="position">The origin of the soft body</param>
	/// <param name="spacing">The distance between the origins of the spheres</param>
	/// <param name="springForce">The strength of the spring</param>
	/// <param name="damping">The dampning of the springs</param>
	/// <param name="strings">A string array determining where the spheres should be placed on a grid. 0 represents circle, . represents space</param>
	static void Build(PhysicsScene* scene, glm::vec2 position, float spacing, float springForce, float damping, std::vector<std::string>& strings);
};

