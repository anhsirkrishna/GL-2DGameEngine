#pragma once

#include "GameObjectManager.h"

#include <glm.hpp>

/* A class for collision detection and management */
class CollisionManager
{

public:

	// Default ctor
	CollisionManager() {};

	// Default dtor
	~CollisionManager() {};

	/* To check if two game objects are colliding. (AABB)
	 * Returns true if colliding, else returns false
	 */
	bool Colliding(glm::vec4 pos0, glm::vec4);

	// Function to constantly keep checking for any possible collisions
	void Update();
};