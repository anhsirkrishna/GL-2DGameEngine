#include "ControlSchemeManager.h"
#include "InputManager.h"
#include <SDL.h>
#include <fstream>

ControlSchemeManager::ControlSchemeManager() {

	// Set up control map
	Serialize();

	// Everything is released when the game starts
	action_control_state[Action::MOVE_LEFT] = ControlState::INACTIVE;
	action_control_state[Action::MOVE_RIGHT] = ControlState::INACTIVE;
	action_control_state[Action::JUMP] = ControlState::INACTIVE;
}

void ControlSchemeManager::Serialize() {

	/* Get data from Control_Config.json (under the Settings folder)
	 * And load them into a json object variable
	 */
	std::string control_config_file = ".\\Settings\\Control_Config.json";
	std::ifstream control_config_data(control_config_file);

	json json_object;
	control_config_data >> json_object;

	// Get the map of action names to vector of vector of codes
	auto control_map = json_object.get<std::unordered_map<std::string, json>>();

	Action action_enum_index;
	std::vector<std::vector<int>> control_code_struct_vec;

	// Iterate through map
	for (auto element : control_map) {

		/* Enum key index is the enum value of that action.
		 * E.g. Enum of the action of Moving left is Action::MOVE_LEFT which is 0.
		 * So Enum Key for this is 0.
		 */
		action_enum_index = static_cast<Action>(element.second["enum_key"].get<int>());

		/* Get the vector of vectors of codes,
		 * Where the vector of codes = values going into a CodeStruct
		 * That struct is then added to the vector for in the map for key = enum key
		 */
		control_code_struct_vec = element.second["control_codes"].get<std::vector<std::vector<int>>>();
		for (int i = 0; i < control_code_struct_vec.size(); i++) {
			CodeStruct codeSet;
			codeSet.keyboard_key_code = control_code_struct_vec[i][0];
			codeSet.controller_button_code = control_code_struct_vec[i][1];
			codeSet.joystick_axis_code = control_code_struct_vec[i][2];
			codeSet.joystick_axis_value = control_code_struct_vec[i][3];
			action_control_map[action_enum_index].push_back(codeSet);
		}
	}
}

void ControlSchemeManager::Update() {

	// Iterate through action map
	for (auto &a : action_control_map) {

		// Iterate over all codes defined for each action.
		for (CodeStruct& c : a.second) {

			/* Check for different types of input for each action
			 * Set the sates of the corresponding action accordingly 
			 */
			if (p_input_manager->isKeyPressed(c.keyboard_key_code) ||
				p_input_manager->isControllerButtonPressed(c.controller_button_code) ||
				p_input_manager->getAxisValueAt(c.joystick_axis_code) == c.joystick_axis_value)  {

				action_control_state[a.first] = ControlState::HELD;
			}

			if (p_input_manager->isKeyTriggered(c.keyboard_key_code) ||
				p_input_manager->isControllerButtonTriggered(c.controller_button_code)) {

				action_control_state[a.first] = ControlState::TRIGGERED;
			}

			if (p_input_manager->isKeyReleased(c.keyboard_key_code) ||
				p_input_manager->isControllerButtonReleased(c.controller_button_code) ||
				p_input_manager->isControllerAxisReleased(c.joystick_axis_code)) {

				action_control_state[a.first] = ControlState::RELEASED;
			}
		}

		// If state of the action was RELEASED in the prev. frame, set it to INACTIVE
		if (action_control_state_prev[a.first] == ControlState::RELEASED) {
			action_control_state[a.first] = ControlState::INACTIVE;
		}
	}

	// Copy the states to another structure. Stores previous states
	for (auto& a : action_control_state) {
		action_control_state_prev[a.first] = a.second;
	}

}

std::unordered_map<Action, ControlState> ControlSchemeManager::GetActionStateMap() {
	return action_control_state;
}
