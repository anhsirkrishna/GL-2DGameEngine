/******************************************************************************/
/* !
/* File   Rigidbody.h
/* Author Sreyash Raychaudhuri
/* Email: srey.raychaudhuri@digipen.edu
/* Date   01/27/2022
/* Rigidbody Component class encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "Component.h"
#include "Transform.h"


#include <glm.hpp>

class Rigidbody : public Component {
public:

	// Default ctor for Rigidbody
	Rigidbody();

	// Returns the mass value
	float GetMass();

	// Returns the velocity
	glm::vec4 GetVelocity();

	// Sets the velocity
	void SetVelocity(glm::vec4 new_velocity);

	// Decide whether to use gravity or not for owner object
	void SetGravityUsage(bool gravity_on_or_off);

	// Update the transform component of the owner game object
	void Update();

	// Stores references to other components
	void Link();

private:
	bool gravity_on;
	float mass;
	glm::vec4 velocity;
	Transform* p_owner_transform;
};

