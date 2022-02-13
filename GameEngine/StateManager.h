/******************************************************************************/
/*!
/*File   StateManager.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   02/10/2022
/*  StateManager header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#pragma once
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class GameObject;

class StateManager {
private:
	std::string current_state;
	std::string prev_state;
	std::unordered_map<std::string, json> states;
	GameObject* p_owner_game_object;
public:
	/*Changes to the next state if the next state is different
	* Does nothing if the next state is the same as current state
	* Returns: Void
	*/
	void ChangeState(std::string);
	
	/*Adds a new state to the state manager
	* If state already exists, does nothing.
	* Returns: void
	*/
	void AddState(std::string, json);

	/*Sets a game object as the owner
	* Returns: void
	*/
	void SetOwner(GameObject* p_game_object);
};