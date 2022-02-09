/******************************************************************************/
/*!
/*File   GameObjectFactory.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  GameObjectFactory implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "GameObjectManager.h"
#include "GameObjectFactory.h"
#include <iostream>

/*Create a game object based on the object definition
* Object definition is in a json format found in .\\Obj_defs\\<file_name>
* Obj defs is a dict with the component names as keys.
* Returns  *GameObject
*/
GameObject* GameObjectFactory::CreateGameObject(std::string object_name, std::string obj_def) {
	GameObject* new_object = new GameObject(object_name);

	std::string obj_def_file = ".\\Obj_defs\\" + obj_def + ".json";
	std::ifstream obj_def_data(obj_def_file);

	json json_object;
	obj_def_data >> json_object;
	auto component_map = json_object.get<std::unordered_map<std::string, json>>();

	json component_data;
	std::string component_name;
	std::string state_name;
	for (auto component : component_map) {
		component_name = component.first;
		component_data = component.second;
		new_object->AddComponent(
			component_factory.Create(component_name, component_data)
		);
	}
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
	}

	for (auto game_obj : p_game_obj_manager->game_object_list)
		game_obj->LinkComponents();
}
