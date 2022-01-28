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
	//Creates a game object
	GameObject* CreateGameObject(std::string object_name, std::string obj_def);
	/*Creates all the game objects for a specific level.
	* Adds each game object to the game object manager
	*/
	void CreateLevel(unsigned int level);
};