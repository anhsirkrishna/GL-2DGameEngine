#include "Controller.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "GameObject.h"
#include "Transform.h"
#include "Movement.h"

#include <SDL.h>

void Controller::Update() {

	//// Move right
	//if (p_input_manager->isKeyPressed(SDL_SCANCODE_L)) {
	//	p_owner_movement->MoveHorizontally(200.0f);
	//}

	//// Move left
	//if (p_input_manager->isKeyPressed(SDL_SCANCODE_J)) {
	//	p_owner_movement->MoveHorizontally(-200.0f);
	//}

	//// Jump
	//if (p_input_manager->isKeyTriggered(SDL_SCANCODE_I)) {
	//	p_owner_movement->SetGravityUsage(true);
	//	p_owner_movement->Jump(-200.0f);
	//}


	//// Stop horizontal movement
	//if (p_input_manager->isKeyReleased(SDL_SCANCODE_L) ||
	//	p_input_manager->isKeyReleased(SDL_SCANCODE_J)) {
	//	p_owner_movement->MoveHorizontally(0);
	//}
}

void Controller::Serialize(json json_object) {
}

void Controller::Link() {
	p_owner_movement = static_cast<Movement*>(GetOwner()->HasComponent("MOVEMENT"));
}
