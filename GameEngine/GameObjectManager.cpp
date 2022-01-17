#include "GameObjectManager.h"

/*
* Adds a game object to the list of game objects
*/
void GameObjectManager::AddGameObject(GameObject* game_object) {
	game_object_list.push_back(game_object);
}

/*
* Calls the Update() function for each game object
*/

void GameObjectManager::Update() {
	for (int i = 0; i < game_object_list.size(); ++i) {
		game_object_list[i]->Update();
	}
}

/*
* Calls the Draw() function for each game object
*/
void GameObjectManager::Draw(ShaderProgram* program) {
	for (int i = 0; i < game_object_list.size(); ++i) {
		game_object_list[i]->Draw(program);
	}
}

/*
* Deletes the game objects and sets the game object list to NULL
*/
void GameObjectManager::Cleanup() {

	for (int i = 0; i < game_object_list.size(); ++i) {
		delete game_object_list[i];
	}
	
	game_object_list.clear();
}


/*
* Deletes the game objects at index i and sets the value to NULL
*/

void GameObjectManager::Delete(unsigned int index) {
	delete game_object_list[index];
	game_object_list.erase(game_object_list.begin() + (index - 1));
}
