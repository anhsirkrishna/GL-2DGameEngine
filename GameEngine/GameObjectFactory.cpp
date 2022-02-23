/******************************************************************************/
/*!
/*File   GameObjectFactory.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  GameObjectFactory implementation file
/* DigiPen Institute of Technology � 2022
/******************************************************************************/

#include "GameObjectManager.h"
#include "GameObjectFactory.h"
#include "MemoryManager.h"
#include "Transform.h"
#include <iostream>

/*Create a game object based on the object definition
* Object definition is in a json format found in .\\Obj_defs\\<file_name>
* Obj defs is a dict with "COMPONENT" and "STATES" as keys.
* "COMPONENT" is another dict with the component names as keys
* "STATES" is another dict with the state names as keys
* Returns  *GameObject
*/
GameObject* GameObjectFactory::CreateGameObject(std::string object_name, std::string obj_def) {
	GameObject* new_object = new GameObject(object_name);

	std::string obj_def_file = ".\\Obj_defs\\" + obj_def + ".json";
	std::ifstream obj_def_data(obj_def_file);

	json json_object;
	obj_def_data >> json_object;
	auto object_map = json_object.get<std::unordered_map<std::string, json>>();

	json component_json = object_map.at("COMPONENTS");
	auto component_map = component_json.get<std::unordered_map<std::string, json>>();
	json component_data;
	std::string component_name;
	for (auto component : component_map) {
		component_name = component.first;
		component_data = component.second;
		new_object->AddComponent(
			component_factory.Create(component_name, component_data)
		);
	}

	auto state_json = object_map.at("STATES");
	auto states_map = state_json.get<std::unordered_map<std::string, json>>();
	json state_data;
	std::string state_name;
	for (auto state : states_map) {
		state_name = state.first;
		state_data = state.second;
		new_object->state_manager.AddState(state_name, state_data);
	}

	std::string default_state = object_map.at("DEFAULT_STATE").get<std::string>();
	new_object->state_manager.ChangeState(default_state);

	return new_object;
}

/*Creates all the game objects for a specific level.
* Adds each game object to the game object manager
*/
void GameObjectFactory::CreateLevel(unsigned int level) {
	std::string level_file = ".\\Levels\\Level_" + std::to_string(level) + ".json";
	std::ifstream level_data(level_file);

	json json_object;
	level_data >> json_object;

	auto object_map = json_object.get<std::unordered_map<std::string, json>>();

	GameObject* new_object;
	std::string obj_def;
	std::string default_state;
	bool is_background;
	for (auto element : object_map) {
		obj_def = element.second["obj_def"].get<std::string>();
		new_object = CreateGameObject(element.first, obj_def);
		p_game_obj_manager->AddGameObject(new_object);
		if (element.second.contains("starting_position")) {
			auto start_pos = element.second["starting_position"].get<std::vector<float>>();
			glm::vec4 new_position(start_pos[0], start_pos[1], start_pos[2], start_pos[3]);
			static_cast<Transform*>(new_object->HasComponent("TRANSFORM"))->SetPosition(new_position);
		}
	}

	for (auto game_obj : p_game_obj_manager->game_object_list)
		game_obj->LinkComponents();
}
