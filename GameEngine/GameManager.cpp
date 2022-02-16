#include "GameManager.h"

//Default Constructor
GameManager::GameManager() : is_running(true), level(0), debug_mode(false) {
}

//Sets the run status to false
void GameManager::Quit() {
	is_running = false;
}

//Returns the current run status
bool GameManager::Status() {
	return is_running;
}

//Returns the current level
unsigned int GameManager::Level() {
	return level;
}

//Sets the current level
void GameManager::SetLevel(int next_level) {
	level = next_level;
}

//Sets the debug mode
void GameManager::SetDebugMode(bool mode_) {
	debug_mode = mode_;
}

//Gets the debug mode
bool GameManager::GetDegugMode() {
	return debug_mode;
}