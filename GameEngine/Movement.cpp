/******************************************************************************/
/* !
/* File   Movement.cpp
/* Author Sreyash Raychaudhuri
*		  Krishna Pillai - Added logic to interact with script
/* Email: srey.raychaudhuri@digipen.edu
/* Date   01/27/2022
/* Movement Component implementation file
/* DigiPen Institute of Technology � 2022
/******************************************************************************/

#include "Movement.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "FrameRateController.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "Events.h"

#include <SDL.h>
#include <glm.hpp>

Movement::Movement() : Component("MOVEMENT"), p_owner_transform(nullptr), mass(5),
						velocity(glm::vec4(0)), gravity_on(false){
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

bool Movement::GetGravity()
{
	return gravity_on;
}

// Sets the new velocity value
void Movement::SetVelocity(glm::vec4 new_velocity)
{
	velocity = new_velocity;
}

// Sets the x velocity
void Movement::MoveHorizontally(float vel_x)
{
	velocity.x = vel_x;
}


// Gives a -ve y velocity.
void Movement::Jump(float vel_y) 
{
	EnableGravity();
	velocity.y = -vel_y;

	p_audio_manager->Play("jumping.wav");

}

// Serialize method. Nothing to do for the movement component
void Movement::Serialize(json json_object)
{
	gravity_capable = json_object["gravity_on"].get<bool>();
	gravity_on = gravity_capable;
}

// Stores references to other components
void Movement::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}

//Returns the velocity in the y direction
float Movement::GetHorizontalVelocity() {
	return velocity.x;
}


//Returns the velocity in the y direction
float Movement::GetVerticalVelocity() {
	return velocity.y;
}

void Movement::EnableGravity() {
	gravity_on = true;
}

void Movement::DisableGravity() {
	gravity_on = false;
}

void Movement::Reset() {
	velocity = glm::vec4(0);
	gravity_on = gravity_capable;
}

bool Movement::IsGravityCapable() {
	return gravity_capable;
}

/*Handles events
* Returns: void
*/
void Movement::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id)
	{
	case EventID::die:
		DisableGravity();
		break;
	case EventID::disable:
		DisableGravity();
		break;
	default:
		break;
	}
}

void Movement::ToggleInfiniteJump()
{
	infinite_jump = !infinite_jump;
}

bool Movement::CanInfiniteJump()
{
	return infinite_jump;
}
