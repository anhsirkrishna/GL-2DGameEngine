#pragma once
#include "GameObject.h"
#include <vector>

class GameObjectManager {
public:
	/*
	* Adds a game object to the list of game objects
	*/
	void AddGameObject(GameObject* game_object);

	/*
	* Adds a game object to the list of game objects
	* in a specific order
	*/
	void AddGameObject(GameObject* game_object, int order);

	/*
	* Calls the Update() function for each game object
	*/
	void Update();

	/*
	* Calls the Update() function for each game object
	* starting from the start_index
	*/
	void Update(unsigned int start_index);

	/*
	* Calls the Draw() function for each game object
	*/
	void Draw(ShaderProgram* program);

	/*
	* Deletes the game objects and sets the game object list to NULL
	*/
	void Cleanup();

	/*
	* Deletes the game objects at index i and sets the value to NULL
	*/
	void Delete(unsigned int index);

	/*
	* Max number of objects in the game at a particular time
	*/
	const unsigned int max_objects = 50;


	std::vector<GameObject*> game_object_list;
};

//Extern global variable defined so it can be accessed throughout the project
extern GameObjectManager* p_game_obj_manager;
extern GameObjectManager* p_ui_obj_manager;