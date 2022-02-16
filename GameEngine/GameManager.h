#pragma once

/*
* The GameManager class is used to manage the game at the highest level.
* Maintains its run status, current level, debug mode etc.
*/
class GameManager {
private:
	bool is_running;
	int level;
public:
	bool debug_mode;
	//Default Constructor
	GameManager();

	//Sets the run status to false
	void Quit();

	//Returns the current run status
	bool Status();

	//Returns the current level
	unsigned int Level();

	//Sets the current level
	void SetLevel(int level);

	//Sets the debug mode
	void SetDebugMode(bool mode_);

	//Gets the debug mode
	bool GetDegugMode();
};

//Declared as an extern variable so it can be accessed throughout the project
extern GameManager * p_game_manager;
