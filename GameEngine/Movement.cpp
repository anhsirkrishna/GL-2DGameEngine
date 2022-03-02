/******************************************************************************/
/* !
/* File   Movement.cpp
/* Author Sreyash Raychaudhuri
*		  Krishna Pillai - Added logic to interact with script
/* Email: srey.raychaudhuri@digipen.edu
/* Date   01/27/2022
/* Movement Component implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "Movement.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"

#include <SDL.h>
#include <glm.hpp>

Movement::Movement() : Component("MOVEMENT"), p_owner_transform(nullptr),
						p_owner_collider(nullptr), mass(5),
						velocity(glm::vec4(0)), gravity_on(false),
						dirLocks({false, false, false, false}) {
}

// Returns the mass value
float Movement::GetMass() {
	return mass;
}

// Returns the velocity value
glm::vec4 Movement::GetVelocity()
{
	return velocity;
}

// Sets the new velocity value
void Movement::SetVelocity(glm::vec4 new_velocity)
{
	velocity = new_velocity;
}

// Sets the x velocity
void Movement::MoveHorizontally(float vel_x)
{
	// trying to move leftward and left lock is disabled and 
	// trying to move rightward and right lock is disabled, then only set the velocity
	if (!(vel_x < 0 && dirLocks.left_lock) && !(vel_x > 0 && dirLocks.right_lock)) {
		velocity.x = vel_x;
	}

}


// Gives a -ve y velocity. Min is -10.0f
void Movement::Jump(float vel_y) 
{
	// Has to have a minimum upward velocity of 10.0f;
	if (vel_y > -10.0f) {
		velocity.y = -100.0f;
	}
	else {
		velocity.y = vel_y;
	}

	dirLocks.down_lock = false;
}

// Serialize method. Nothing to do for the movement component
void Movement::Serialize(json json_object)
{
	gravity_on = json_object["gravity_on"].get<bool>();
}

// Stores references to other components
void Movement::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	p_owner_collider = static_cast<Collider*>(GetOwner()->HasComponent("COLLIDER"));
}


//Returns the velocity in the y direction
float Movement::GetVerticalVelocity() {
	return velocity.y;
}

//Returns downlock
bool Movement::GetDownLock() {
	return dirLocks.down_lock;
}