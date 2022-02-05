#include "Rigidbody.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "GameObject.h"

#include <SDL.h>
#include <glm.hpp>

extern FrameRateController* p_framerate_controller;

Rigidbody::Rigidbody() : Component("RIGIDBODY"), p_owner_transform(nullptr), gravity(100), friction(250), force(500), mass(5), velocity(glm::vec4(0)) {}

//Returns the force value
float Rigidbody::GetForce(){
	return force;
}

//Returns the mass value
float Rigidbody::GetMass() {
	return mass;
}

//Returns the gravity value
float Rigidbody::GetGravity()
{
	return gravity;
}

//Returns the friction value
float Rigidbody::GetFriction()
{
	return friction;
}

//Returns the velocity value
glm::vec4 Rigidbody::GetVelocity()
{
	return velocity;
}

//Sets the new velocity value
void Rigidbody::SetVelocity(glm::vec4 new_velocity)
{
	velocity = new_velocity;
}

/* Update the velocity according to the given paramaters
* arg 1: index 0 or 1 corresponding to x or y axis for glm vec
* arg 2: add (1) or subtract (-1)
* arg 3: use force or friction
*/
void Rigidbody::UpdateVelocity(int coord_axis_index, int add_or_sub, std::string forceType)
{
	if (forceType == "FORCE") {
		velocity[coord_axis_index] += add_or_sub * (force / mass) * p_framerate_controller->GetPrevLoopDeltaTime() / 1000.0f;
	}
	else {
		velocity[coord_axis_index] += add_or_sub * (friction / mass) * p_framerate_controller->GetPrevLoopDeltaTime() / 1000.0f;
	}
}

// Update the transform componenent of the owner game object
void Rigidbody::UpdateTransform()
{
	glm::vec4 pos = p_owner_transform->GetPosition();

	pos.x += velocity.x * p_framerate_controller->GetPrevLoopDeltaTime() / 1000.0f;
	pos.y += velocity.y * p_framerate_controller->GetPrevLoopDeltaTime() / 1000.0f;

	p_owner_transform->SetPosition(pos);
}

void Rigidbody::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}


