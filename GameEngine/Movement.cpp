#include "Movement.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "GameObject.h"
#include "Transform.h"

#include <SDL.h>
#include <glm.hpp>

unsigned int gravity = 200;

Movement::Movement() : Component("MOVEMENT"), p_owner_transform(nullptr),
					mass(5), velocity(glm::vec4(0)), gravity_on(false) {}

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

// Decide whether to use gravity or not for owner object
void Movement::SetGravityUsage(bool gravity_on_or_off)
{
	gravity_on = gravity_on_or_off;
}

// Get the value of the gravity switch 
bool Movement::GetGravityUsage()
{
	return gravity_on;
}

// Update the transform componenent of the owner game object
void Movement::Update()
{
	float dt = p_framerate_controller->GetPrevLoopDeltaTime() / 1000.0f;

	// Gravity acts on the body if gravity is on
	if (gravity_on) {
		velocity.y += gravity * dt;
	}

	// Update position every frame
	glm::vec4 pos = p_owner_transform->GetPosition();

	pos.x += velocity.x * dt;
	pos.y += velocity.y * dt;

	p_owner_transform->SetPosition(pos);
}


// Sets the x velocity
void Movement::MoveHorizontally(float vel_x)
{
	velocity.x = vel_x;
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
}


