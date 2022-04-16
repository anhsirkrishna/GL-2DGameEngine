/******************************************************************************/
/*!
/*File   CreditsState.h
/*Author Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   04/05/2022
/*  CreditsState header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include "BaseState.h"
#include <vector>

class GameObject;
class LogoScreen;

class CreditsState : public BaseState {
private:
	std::vector<GameObject*> logo_screen;
	std::vector<LogoScreen*> logo_screen_comp;
	std::vector<GameObject*> delete_objs;
public:
	/*Initialize the play state
	* Create the level and all the objects
	* within the level
	*/
	CreditsState();
	/*Deletes the play state by
	* clearing the level and cleaning
	* up all the objects
	*/
	virtual ~CreditsState();
	/*Enters the state from another state
	* Doesn't need any special action
	* Returns: void
	*/
	virtual void Update();
	/*Calls the render/draw call for all the objects
	* Returns: void
	*/
	virtual void Render();
};