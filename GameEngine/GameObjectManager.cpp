#include "GameObjectManager.h"

/*
* Adds a game object to the list of game objects
*/
void GameObjectManager::AddGameObject(GameObject* game_object) {
	for (int i = 0; i < max_objects; i++) {
		if (game_object_list[i] == NULL) {
			game_object_list[i] = game_object;
			game_object->index = i;
			return;
		}
	}
}

/*
* Calls the Update() function for each game object
*/

void GameObjectManager::Update() {
	for (int i = 0; i < max_objects; i++) {
		if (game_object_list[i] == NULL)
			continue;

		game_object_list[i]->Update();
	}
}

/*
* Calls the Draw() function for each game object
*/
void GameObjectManager::Draw(ShaderProgram* program) {
	for (int i = 0; i < max_objects; i++) {
		if (game_object_list[i] == NULL)
			continue;

		game_object_list[i]->Draw(program);
	}
}

/*
* Deletes the game objects and sets the game object list to NULL
*/
void GameObjectManager::Cleanup() {
	for (int i = 0; i < max_objects; i++) {
		if (game_object_list[i] == NULL)
			continue;

		delete game_object_list[i];
		game_object_list[i] = NULL;
	}
}


/*
* Deletes the game objects at index i and sets the value to NULL
*/

void GameObjectManager::Delete(unsigned int index) {
	delete game_object_list[index];
	game_object_list[index] = NULL;
}
