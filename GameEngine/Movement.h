/******************************************************************************/
/* !
/* File   Movement.h
/* Author Sreyash Raychaudhuri
/* Email: srey.raychaudhuri@digipen.edu
/* Date   01/27/2022
/* Movement Component class encapsulation header file
/* DigiPen Institute of Technology � 2022
/******************************************************************************/

#pragma once

#include "Component.h"

#include <glm.hpp>

// Forward declaration
class Transform;
class Collider;

using json = nlohmann::json;


struct DirLocks {
	bool up_lock, down_lock, left_lock, right_lock;
};

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

	// Just check if any directional locks can be disabled if in case enabled
	void Update();

	// Sets the x velocity
	void MoveHorizontally(float vel_x);

	// Gives a -ve y velocity. Min is -10.0f
	void Jump(float vel_y);

	// Serialize method. Nothing to do for the movement component
	void Serialize(json json_object);

	// Stores references to other components
	void Link();

	/* to lock or unlock movement in different directions
	* 1st elem: Upward movement lock: vel y cannot be set to a -ve value and will remain zero
	* 2nd elem: Downward movement lock: vel y cannot be set to a +ve value and will remain zero
	* 3rd elem: Leftward movement lock: vel x cannot be set to a -ve value and will remain zero
	* 4th elem: Righward movement lock: vel x cannot be set to a +ve value and will remain zero
	*/

	DirLocks dirLocks;

private:

	bool gravity_on;
	float mass;
	glm::vec4 velocity;
	Transform* p_owner_transform;
	Collider* p_owner_collider;
};

