#include "Controller.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"

#include <SDL.h>

extern InputManager* p_input_manager;

void Controller::Update(float delta_time)
{
	//Get refrences to the owner game object to get access to the tranform and p_owner_rigidbody components
	//GameObject* owner_game_object = GetOwner();
	//Transform* transform = static_cast<Transform*>(owner_game_object->HasComponent("TRANSFORM"));
	//p_owner_rigidbody* p_owner_rigidbody = static_cast<p_owner_rigidbody*>(owner_game_object->HasComponent("p_owner_rigidbody"));

	//glm::vec4 new_position = transform->GetPosition();

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

	//Update position

	//new_position.x += velocity.x * delta_time;
	//new_position.y += velocity.y * delta_time;

	//transform->SetPosition(new_position);

}

void Controller::Link()
{
	p_owner_rigidbody = static_cast<Rigidbody*>(GetOwner()->HasComponent("RIGIDBODY"));
}
