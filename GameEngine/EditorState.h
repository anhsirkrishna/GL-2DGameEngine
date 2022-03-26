/******************************************************************************/
/*!
/*File   EditorState.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  EditorState header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include "BaseState.h"
#include <vector>

class EditorState : public BaseState {
public:
	/*Initialize the play state
	* Create the level and all the objects
	* within the level
	*/
	EditorState();
	/*Deletes the play state by
	* clearing the level and cleaning
	* up all the objects
	*/
	~EditorState();
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
	/*Basic update call for the EditorState
	* Calls Update on all the gameobjects
	* Calls Update on all the required managers
	* Returns: void
	*/
	virtual void Update();
	/*Calls the render/draw call for all the objects
	* Returns: void
	*/
	virtual void Render();
};