#include "Controller.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"

#include <SDL.h>

void Controller::Update()
{
	glm::vec4 velocity = p_owner_rigidbody->GetVelocity();
	float mass = p_owner_rigidbody->GetMass();
	float dt = p_framerate_controller->GetPrevLoopDeltaTime()/1000.0f;

	// Move right
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_D)) {
		velocity.x = 50.0f;
	}

	// Move left
	if (p_input_manager->isKeyPressed(SDL_SCANCODE_A)) {
		velocity.x = -50.0f;
	}

	// Jump
	if (p_input_manager->isKeyTriggered(SDL_SCANCODE_W)) {
		p_owner_rigidbody->SetGravityUsage(true);
		velocity.y = -100.0f;
	}


	// Stop horizontal movement
	if (p_input_manager->isKeyReleased(SDL_SCANCODE_D) || 
	    p_input_manager->isKeyReleased(SDL_SCANCODE_A)) {
		velocity.x = 0.0f;
	}

	p_owner_rigidbody->SetVelocity(velocity);

	//p_owner_rigidbody->Update();
}

void Controller::Link()
{
	p_owner_rigidbody = static_cast<Rigidbody*>(GetOwner()->HasComponent("RIGIDBODY"));
}
