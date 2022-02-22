/******************************************************************************/
/* !
/* File:   ControlSchemeManager.h
/* Author: Sreyash Raychaudhuri
/* Email: srey.raychaudhuri@digipen.edu
/* Date:   02/17/2022
/* Control Scheme Manager encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/* Data in this struct :
 * 1st member: SDL Key Code for keyboard I/P
 * 2nd member: SDL Controller Button Code for contr. btn. I/P
 * 3rd member: SDL Code for the joystick axis
 * 4th member: Value for the joystick axis
 */
struct CodeStruct {
	int keyboard_key_code; 
	int controller_button_code;
	int joystick_axis_code;
	int joystick_axis_value;
};

// All possible actions. More to be added soon
enum class Action {
	MOVE_LEFT,
	MOVE_RIGHT,
	JUMP
};

// States of action controls
enum class ControlState {
	HELD,
	TRIGGERED,
	RELEASED
};

class ControlSchemeManager
{
public:
	ControlSchemeManager();

	/* Set up control mapping by reading values from
	 * The control config. JSON file
	 */
	void Serialize();

	// Update function for calling
	void Update();

	// Getter to get the action map state
	std::unordered_map<Action, ControlState> GetActionStateMap();

private:
	// map of actions to different types of inputs
	std::unordered_map<Action, std::vector<CodeStruct>> action_control_map;

	// map of actions to key state
	std::unordered_map<Action, ControlState> action_control_state;
};

extern ControlSchemeManager* p_control_scheme_manager;

