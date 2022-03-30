/******************************************************************************/
/*!
/*File   GameObjectFactory.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  GameObjectFactory header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include "GameObject.h"
#include "ComponentFactory.h"

#include <fstream>

/*GameObjectFactory is responsible for reading a level file and 
* creating a 'GameObject' object for each entry in the level file
*/
class GameObjectFactory {
private:
	ComponentFactory component_factory;
public:
	/*Create a game object based on the object definition
	* Object definition is in a json format found in .\\Obj_defs\\<file_name>
	* Obj defs is a dict with "COMPONENT" and "STATES" as keys.
	* "COMPONENT" is another dict with the component names as keys
	* "STATES" is another dict with the state names as keys
	* Returns  *GameObject
	*/
	GameObject* CreateGameObject(std::string object_name, std::string obj_def);

	/*Creates all the game objects for a specific level.
	* Adds each game object to the game object manager
	*/
	std::unordered_map<std::string, json> CreateLevel(unsigned int level);

	/*Create a UI object based on the object definition
	* UI Obj defs is a dict with "UI_COMPONENT_NAMES" keys.
	* "UI_COMPONENT_NAMES" is another dict with the component names as keys
	* Returns  *GameObject
	*/
	GameObject* CreateUIObject(std::string object_name, json json_object);

	/*Creates all the game objects for a specific level.
	* Adds each game object to the game object manager
	*/
	std::unordered_map<std::string, json> CreateUI(std::string ui_def);
};