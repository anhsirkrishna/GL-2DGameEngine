#include "Controller.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"

#include <SDL.h>

extern InputManager* p_input_manager;

void Controller::Update(float delta_time)
{
	//Accelerate right
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_D)) {
		p_owner_rigidbody->UpdateVelocity(0, 1, "FORCE", delta_time);
	}

	//Accelerate left
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_A)) {
		p_owner_rigidbody->UpdateVelocity(0, -1, "FORCE", delta_time);
	}

	//Accelerate upward
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_W)) {
		p_owner_rigidbody->UpdateVelocity(1, -1, "FORCE", delta_time);
	}

	//Accelerate downward
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_S)) {
		p_owner_rigidbody->UpdateVelocity(1, 1, "FORCE", delta_time);
	}

	glm::vec4 velocity = p_owner_rigidbody->GetVelocity();

	// if no key is pressed - decelarate
	// && check if absolute val of velocity > 0
	if ( (fabsf(velocity.x) > 0.0 || fabsf(velocity.y) > 0.0)
		&& !p_input_manager->isKeyPressed(SDL_SCANCODE_A)
		&& !p_input_manager->isKeyPressed(SDL_SCANCODE_D)
		&& !p_input_manager->isKeyPressed(SDL_SCANCODE_W)
		&& !p_input_manager->isKeyPressed(SDL_SCANCODE_S)) {

		// if moving leftward, friction is applied rightward
		if (velocity.x < 0.0f) {				
			p_owner_rigidbody->UpdateVelocity(0, 1, "FRICTION", delta_time);
		}
		// if moving rightward, friction is applied rightward
		else {
			p_owner_rigidbody->UpdateVelocity(0, -1, "FRICTION", delta_time);
		}

		// if moving upward, friction is applied downward
		if (velocity.y < 0.0f) {
			p_owner_rigidbody->UpdateVelocity(1, 1, "FRICTION", delta_time);
		}
		// if moving downward, friction is applied upward
		else {
			p_owner_rigidbody->UpdateVelocity(1, -1, "FRICTION", delta_time);
		}
	}

	p_owner_rigidbody->UpdateTransform(delta_time);
}

void Controller::Link()
{
	p_owner_rigidbody = static_cast<Rigidbody*>(GetOwner()->HasComponent("RIGIDBODY"));
}
