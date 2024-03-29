/******************************************************************************/
/*!
/*File   GamePlayState.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  BaseState header file
/* DigiPen Institute of Technology � 2022
/******************************************************************************/

#pragma once
#include "BaseState.h"
#include <vector>

class GameObject;
class Health;

class PlayState : public BaseState {
private:
	GameObject* player_obj;
	std::vector<Health*> enemy_obj_health_list;
public:
	/*Initialize the play state
	* Create the level and all the objects 
	* within the level
	*/
	PlayState();
	/*Deletes the play state by
	* clearing the level and cleaning
	* up all the objects
	*/
	virtual ~PlayState();
	/*Enters the state from another state
	* Doesn't need any special action
	* Returns: void
	*/
	virtual void Enter();
	/*Exits the state into another state
	* Doesn't need any special action
	* Returns: void
	*/
	virtual void Exit();
	/*Basic update call for the PlayState
	* Calls Update on all the gameobjects
	* Calls Update on all the required managers
	* Returns: void
	*/
	virtual void Update();
	/*Calls the render/draw call for all the objects
	* Returns: void
	*/
	virtual void Render();

	/*Restart the level
	* Returns void
	*/
	virtual void Reset();

	/*Check if all the enemy objects are dead
	* if they are dead then mark as win
	* Return : bool
	*/
	bool CheckWinCondition();
};