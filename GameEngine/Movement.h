/******************************************************************************/
/* !
/* File   Movement.h
/* Author Sreyash Raychaudhuri
*		  Krishna Pillai - Added logic to interact with script
/* Email: srey.raychaudhuri@digipen.edu
/* Date   01/27/2022
/* Movement Component class encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "Component.h"

#include <glm.hpp>

// Forward declaration
class Transform;
class Collider;

using json = nlohmann::json;

class Movement : public Component {
private:
	bool gravity_on;
	float mass;
	glm::vec4 velocity;
	Transform* p_owner_transform;

public:

	// Default ctor for Rigidbody
	Movement();

	// Returns the mass value
	float GetMass();

	// Returns the velocity
	glm::vec4 GetVelocity();

	// Get the gravity values
	bool GetGravity();

	// Sets the velocity
	void SetVelocity(glm::vec4 new_velocity);

	// Sets the x velocity
	void MoveHorizontally(float vel_x);

	// Gives a -ve y velocity. Min is -10.0f
	void Jump(float vel_y);

	// Serialize method. Nothing to do for the movement component
	void Serialize(json json_object);

	// Stores references to other components
	void Link();

	//Returns the velocity in the x direction
	float GetHorizontalVelocity();

	//Returns the velocity in the y direction
	float GetVerticalVelocity();

	/*Enable gravity
	* Returns: void
	*/
	void EnableGravity();

	/*Disable gravity
	* Returns: void
	*/
	void DisableGravity();

	/*Reset the parameters for movement
	* Returns: void
	*/
	virtual void Reset();

	/*Handles events
	* Returns: void
	*/
	virtual void HandleEvent(TimedEvent* p_event);
};

