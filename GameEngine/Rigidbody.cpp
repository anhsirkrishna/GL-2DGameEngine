#include "Rigidbody.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "GameObject.h"

#include <SDL.h>
#include <glm.hpp>

#define gravity 200

extern FrameRateController* p_framerate_controller;

Rigidbody::Rigidbody() : Component("RIGIDBODY"), p_owner_transform(nullptr), mass(5), velocity(glm::vec4(0)), gravity_on(true) {}

// Returns the mass value
float Rigidbody::GetMass() {
	return mass;
}

// Returns the velocity value
glm::vec4 Rigidbody::GetVelocity()
{
	return velocity;
}

// Sets the new velocity value
void Rigidbody::SetVelocity(glm::vec4 new_velocity)
{
	velocity = new_velocity;
}

// Decide whether to use gravity or not for owner object
void Rigidbody::SetGravityUsage(bool gravity_on_or_off)
{
	gravity_on = gravity_on_or_off;
}

// Update the transform componenent of the owner game object
void Rigidbody::Update()
{
	if (GetOwner()->GetName() == "demo_obj" && gravity_on) {
		velocity.y += gravity * p_framerate_controller->GetPrevLoopDeltaTime() / 1000.0f;
	}

	glm::vec4 pos = p_owner_transform->GetPosition();

	pos.x += velocity.x * p_framerate_controller->GetPrevLoopDeltaTime() / 1000.0f;
	pos.y += velocity.y * p_framerate_controller->GetPrevLoopDeltaTime() / 1000.0f;

	p_owner_transform->SetPosition(pos);
}

// Stores references to other components
void Rigidbody::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}


