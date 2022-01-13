#pragma once
#include "GameObject.h"

class GameObjectManager {
public:
	/*
	* Adds a game object to the list of game objects
	*/
	void AddGameObject(GameObject* game_object);

	/*
	* Calls the Update() function for each game object
	*/
	void Update();

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


	GameObject* game_object_list[50];
};

//Extern global variable defined so it can be accessed throughout the project
extern GameObjectManager* p_game_obj_manager;
