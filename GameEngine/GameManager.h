#pragma once

/*
* The GameManager class is used to manage the game at the highest level.
* Maintains its run status, current level, debug mode etc.
*/
class GameManager {
private:
	bool is_running;
	int level;
	bool debug_mode;
public:
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
};

//Decaled as an extern variable so it can be accessed throughout the project
extern GameManager * p_game_manager;
