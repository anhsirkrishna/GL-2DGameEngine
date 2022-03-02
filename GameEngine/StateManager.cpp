/******************************************************************************/
/*!
/*File   StateManager.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/10/2022
/*  StateManager header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "StateManager.h"
#include "GameObject.h"
#include "Component.h"

#include <SDL.h>

/*Changes to the next state if the next state is different
* Does nothing if the next state is the same as current state
* Returns: Void
*/
void StateManager::ChangeState(std::string next_state_) {
	//SDL_Log("Change state called for %s", next_state_.c_str());
	if (current_state == next_state_)
		return;

	std::string component_name;
	json next_state_def_ = states.at(next_state_);
	auto component_map = 
		next_state_def_.get<std::unordered_map<std::string, json>>();

	//Iterate through each component that needs to be changed
	//Not all components of the game object need to have their state changed
	for (auto component : component_map) {
		component_name = component.first;
		(p_owner_game_object->HasComponent(component_name))
			->ChangeState(component.second);
	}

	prev_state = current_state;
	current_state = next_state_;
}

/*Adds a new state to the state manager
* If state already exists, does nothing.
* Returns: void
*/
void StateManager::AddState(std::string new_state_, json json_object_) {
	if (states.find(new_state_) == states.end()) {
		states.insert({ new_state_, json_object_ });
	}
}

/*Sets a game object as the owner
	* Returns: void
	*/
void StateManager::SetOwner(GameObject* p_game_object) {
	p_owner_game_object = p_game_object;
}

/*Get the current state
* Returns : string
*/
std::string StateManager::GetCurrentState() {
	return current_state;
}

/*Get the previous state
* Returns : string
*/
std::string StateManager::GetPrevState() {
	return prev_state;
}