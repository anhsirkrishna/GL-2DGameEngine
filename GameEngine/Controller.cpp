#include "Controller.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "GameObject.h"
#include "Transform.h"
#include "Movement.h"
#include "ControlSchemeManager.h"

#include <SDL.h>

void Controller::Update() {

	std::unordered_map<Action, ControlState> actionState =
		p_control_scheme_manager->GetActionStateMap();

	// Move right
	if (actionState[Action::MOVE_RIGHT] == ControlState::HELD) {
		p_owner_movement->MoveHorizontally(200.0f);
	}

	// Move left
	if (actionState[Action::MOVE_LEFT] == ControlState::HELD) {
		p_owner_movement->MoveHorizontally(-200.0f);
	}

	// Jump
	if (actionState[Action::JUMP] == ControlState::TRIGGERED ) {
		p_owner_movement->Jump(-200.0f);
	}


	// Stop horizontal movement
	if (actionState[Action::MOVE_LEFT] == ControlState::RELEASED ||
		actionState[Action::MOVE_RIGHT] == ControlState::RELEASED) {
		p_owner_movement->MoveHorizontally(0);
	}
}

void Controller::Serialize(json json_object) {
}

void Controller::Link() {
	p_owner_movement = static_cast<Movement*>(GetOwner()->HasComponent("MOVEMENT"));
}
