/******************************************************************************/
/* !
/* File   Movement.h
/* Author Sreyash Raychaudhuri
/* Email: srey.raychaudhuri@digipen.edu
/* Date   01/27/2022
/* Movement Component class encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "Component.h"

#include <nlohmann/json.hpp>
#include <glm.hpp>

// Forward declaration
class Transform;

using json = nlohmann::json;

class Movement : public Component {
public:

	// Default ctor for Rigidbody
	Movement();

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

	// Sets the x velocity
	void MoveHorizontally(float vel_x);

	// Gives a -ve y velocity. Min is -10.0f
	void Jump(float vel_y);

	// Serialize method. Nothing to do for the movement component
	void Serialize(json json_object);

	// Stores references to other components
	void Link();

private:
	bool gravity_on;
	float mass;
	glm::vec4 velocity;
	Transform* p_owner_transform;
};

